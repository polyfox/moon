#include "moon/intern.h"
#include "moon/audio/libsoundio/audio.hxx"
#include <vector>

struct FrameBuffer {
	// Some kind of internal counter to help with timing
	uint32_t counter;
	int channels;
	// how many frames are actually needed by the request, this is ALWAYS half the length of the actual data size
	// Note that a single frame is equal to `sizeof(float) * channels`
	size_t frameCount;
	// actual data size, do not use this
	size_t dataLength;
	float* frames;
	float sampleRate;

	FrameBuffer() {
		channels = 0;
		frameCount = 0;
		dataLength = 0;
		frames = NULL;
		sampleRate = 0;
	};

	void freeBuffer() {
		if (frames) {
			free(frames);
			frames = NULL;
		}
	}

	void refreshBuffer(size_t minimumBufferLength) {
		if (!frames || dataLength < minimumBufferLength) {
			freeBuffer();
			dataLength = minimumBufferLength;
			frames = (float*)moon_malloczero(sizeof(float) * dataLength);
		}
	}
};

class AbstractVoice {
public:
	bool active;
	int frameCounter;

	virtual void getFrames(const FrameBuffer& data) {
	};

	virtual void reset() {
		active = false;
		frameCounter = 0;
	}
};

class SineVoice : public AbstractVoice {
public:
	float frequency;
	float velocity;

	void getFrames(const FrameBuffer& data) {
		if (frequency > 0)
		{
			for (size_t i = 0; i < data.frameCount; ++i) {
				const float result = sinf((2 * M_PI * (frameCounter + i) * frequency) / data.sampleRate) * velocity;
				for (size_t channel = 0; channel < data.channels; ++channel) {
					data.frames[i + channel] = result;
				}
			}
		}
	}

	void reset() {
		AbstractVoice::reset();
		frequency = 0.0f;
		velocity = 0.0f;
	}
};

//class SampleVoice : public AbstractVoice {
//	float getSample(float sampleRate) {
//		// TODO
//		return 0.0f;
//	}
//};

static std::vector<AbstractVoice*> voices;
static SineVoice sineVoice;
static struct FrameData cacheBuffer;

static void Moon_AudioWrite(struct SoundIoOutStream *outstream, int frameCountMin, int frameCountMax) {
	struct SoundIoChannelArea *areas;
	const struct SoundIoChannelLayout *layout = &outstream->layout;
	const float sampleRate = outstream->sample_rate;
	uint32_t framesLeft = frameCountMax;
	//printf("Doing an audio write: %d\n", frameCountMax);
	//printf("Voices %ld\n", voices.size());
	const size_t minimumBufferLength = framesLeft * layout->channel_count;
	frameBuffer.refreshBuffer(minimumBufferLength);
	frameBuffer.channels = layout->channel_count;
	frameBuffer.sampleRate = sampleRate;

	while (framesLeft > 0) {
		int frameCount = framesLeft;
		int err = soundio_outstream_begin_write(outstream, &areas, &frameCount);
		if (err) {
			// device has died or something worse D;
			printf("Error writing to outstream\n");
			break;
		}
		if (!frameCount) {
			printf("No more frames, jumping ship\n");
			break;
		}

		frameBuffer.frameCount = frameCount;

		// silence output buffer first
		for (int channel = 0; channel < layout->channel_count; ++channel) {
			memset(areas[channel].ptr, 0, areas[channel].step * frameBuffer.frameCount);
		}

		// apply each voice sequentially adding to the output buffer
		for (uint32_t voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex) {
			if (voices[voiceIndex]->active) {
				voices[voiceIndex]->getFrames(frameBuffer);
				// increment the voice's counter so next time it will provide a different stream
				voices[voiceIndex]->frameCounter += frameBuffer.frameCount;
				// write cached frames to output buffer
				// one idea would be to store the frames as slices, though it would be one continous buffer
				// llllll rrrrrr
				// Here we've gone for interleaved (lr lr lr), might perform terribly because of switching back and forth between channels.
				for (uint32_t frameIndex = 0; frameIndex < frameBuffer.frameCount; ++frameIndex) {
					for (int channel = 0; channel < layout->channel_count; ++channel) {
						float* buffer = (float*)(areas[channel].ptr + areas[channel].step * frameIndex);
						float sample = *buffer + frameBuffer.frames[frameIndex + channel];
						// clipping, to avoid overdrive
						*buffer = sample > 1.0f ? 1.0f : (sample < -1.0f ? -1.0f : sample);
					}
				}
			}
		}

		err = soundio_outstream_end_write(outstream);
		if (err) {
			printf("Error ending writing to outstream\n");
			// something terrible happened. >:
			break;
		}
		framesLeft -= frameCount;
	}
}

namespace Moon
{
	struct SoundIo* Audio::m_soundIO = NULL;
	struct SoundIoDevice* Audio::m_device = NULL;
	struct SoundIoOutStream* Audio::m_outStream = NULL;

	Audio::ErrorCode Audio::Initialize()
	{
		m_soundIO = soundio_create();
		// Should I use assert, or set a flag and leave it for mruby to pick up?
		if (!m_soundIO) {
			return Moon::Audio::ErrorCode::MOON_AUDIO_CREATE_ERROR;
		}
		int err = soundio_connect(m_soundIO);
		if (err) {
			return Moon::Audio::ErrorCode::MOON_AUDIO_CONNECTION_ERROR;
		}
		soundio_flush_events(m_soundIO);
		// Determine the default device
		const int defaultDeviceIndex = soundio_default_output_device_index(m_soundIO);
		if (defaultDeviceIndex < 0) {
			return Moon::Audio::ErrorCode::MOON_AUDIO_NO_DEVICE;
		}
		// retrieve the device specified by the index
		m_device = soundio_get_output_device(m_soundIO, defaultDeviceIndex);
		if (!m_device) {
			return Moon::Audio::ErrorCode::MOON_AUDIO_DEVICE_MISSING;
		}
		// create an output stream
		m_outStream = soundio_outstream_create(m_device);
		// 100ms, I hope that's enough
		m_outStream->software_latency = 0.01;
		m_outStream->format = SoundIoFormatFloat32NE;
		m_outStream->write_callback = Moon_AudioWrite;

		// open the stream
		err = soundio_outstream_open(m_outStream);
		if (err) {
			return Moon::Audio::ErrorCode::MOON_AUDIO_COULD_NOT_OPEN_STREAM;
		}
		if (m_outStream->layout_error) {
			// Could be a warning
			//return Moon::Audio::ErrorCode::MOON_AUDIO_STREAM_CHANNEL_LAYOUT_ERROR;
			printf("WARN: Requested channel could not be completed\n");
		}
		sineVoice.active = true;
		sineVoice.velocity = 0.2;
		sineVoice.frequency = 480;
		voices.push_back(&sineVoice);

		err = soundio_outstream_start(m_outStream);
		if (err) {
			return Moon::Audio::ErrorCode::MOON_AUDIO_STREAM_START_ERROR;
			// raise AudioError, "unable to start device"
		}
		//for (int voiceIndex = 0; voiceIndex < AUDIO_VOICE_MAX; ++voiceIndex) {
		//	moonVoices[voiceIndex].reset();
		//}
		// Finally, after bashing your head against the table, you can finally have audio?
		return Moon::Audio::ErrorCode::MOON_AUDIO_OK;
	}

	void Audio::Update()
	{
	}

	void Audio::Terminate()
	{
		soundio_outstream_destroy(m_outStream);
		soundio_device_unref(m_device);
		soundio_destroy(m_soundIO);
		frameBuffer.freeBuffer();
	}
}
