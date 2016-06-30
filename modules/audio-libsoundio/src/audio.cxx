#include "moon/intern.h"
#include "moon/audio/libsoundio/audio.hxx"
#include <vector>

class AbstractVoice {
public:
	bool active;
	int frame;

	virtual float getSample(float sampleRate) {
		return 0.0f;
	};

	virtual void reset() {
		active = false;
		frame = 0;
	}
};

class SineVoice : public AbstractVoice {
public:
	float frequency;
	float velocity;

	float getSample(float sampleRate) {
		if (frequency > 0)
		{
			return sinf((2 * M_PI * frame * frequency) / sampleRate) * velocity;
		}
		return 0.0f;
	}

	void reset() {
		AbstractVoice::reset();
		frequency = 0.0f;
		velocity = 0.0f;
	}
};

class SampleVoice : public AbstractVoice {
	float getSample(float sampleRate) {
		// TODO
		return 0.0f;
	}
};

static std::vector<AbstractVoice*> voices;
static SineVoice sineVoice;

static void Moon_AudioWrite(struct SoundIoOutStream *outstream, int frameCountMin, int frameCountMax) {
	struct SoundIoChannelArea *areas;
	const struct SoundIoChannelLayout *layout = &outstream->layout;
	const float sampleRate = outstream->sample_rate;
	const float secondsPerFrame = 1.0f / sampleRate;
	int framesLeft = frameCountMax;
	printf("Doing an audio write: %d\n", frameCountMax);
	// iterate all the active voices, mix them together and then output to the stream, or something like that
	printf("Voices %d\n", voices.size());
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

		for (int frame = 0; frame < frameCount; ++frame) {
			for (size_t channel = 0; channel < layout->channel_count; ++channel) {
				float* buffer = (float*)(areas[channel].ptr + areas[channel].step * frame);
				float sample = 0.0f;
				for (size_t voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex) {
					if (voices[voiceIndex]->active) {
						sample += voices[voiceIndex]->getSample(sampleRate);
						voices[voiceIndex]->frame += 1;
					}
				}
				// clipping, to avoid overdrive
				*buffer = sample > 1.0f ? 1.0f : (sample < -1.0f ? -1.0f : sample);
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
	}
}