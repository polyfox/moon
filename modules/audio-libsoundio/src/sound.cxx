#include "moon/intern.h"
#include "moon/audio/libsoundio/sound.hxx"

namespace Moon
{
  Sound::Sound(const std::string filename)
  {
    SndfileHandle file(filename, SFM_READ);
  	printf("Opened file '%s'\n", filename.c_str());
	printf("    Sample rate : %d\n", file.samplerate ());
    printf("    Channels    : %d\n", file.channels ());
    printf("    Frames    : %d\n", file.frames());

    channels = file.channels();
    sampleRate = file.samplerate();

    currentFrame = 0;

    int totalSamples = file.frames() * channels * sampleRate;
    source = new float[totalSamples];
    memset(source, 0, totalSamples);

    totalFrames = file.readf(source, file.frames());
    printf("Read n frames: %d\n", totalFrames);
    // TODO: shout if totalFrames didn't match file.frames()
  };


  Sound::~Sound() {
    delete[] source;
  }


  void Sound::mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames) {
    // TODO: compare file.channels() with layout.channel_count
    // handle mono to stereo, vice versa

    // compare how many frames we still have

    for (int frame = 0; frame < frames; ++frame) {
      for (size_t channel = 0; channel < channels; ++channel) {
        float* buffer = (float*)(areas[channel].ptr + areas[channel].step * frame);
        // mix the sample!
        *buffer += source[(currentFrame + frame) * channels + channel];
      }
    }
    currentFrame += frames;
  }
}
