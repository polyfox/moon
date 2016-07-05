#include "moon/intern.h"
#include "moon/audio/libsoundio/music.hxx"

namespace Moon
{
  Music::Music(const std::string filename) :
    file(filename, SFM_READ)
  {
  	printf("Opened file '%s'\n", filename.c_str());
	printf("    Sample rate : %d\n", file.samplerate ());
    printf("    Channels    : %d\n", file.channels ());
  };


  Music::~Music() {
  }


  void Music::mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames) {
    // TODO: compare file.channels() with layout.channel_count
    // handle mono to stereo, vice versa
    int channels = file.channels();
    int totalSamples = frames * channels * file.samplerate();
    float* chunk = new float[totalSamples];
    // clear the array (in case we read less data than available)
    memset(chunk, 0, totalSamples);

    int framesRead = file.readf(cache, frames);
    printf("Read n frames: %d\n", framesRead);

    // TODO: compare frames with framesRead

    for (int frame = 0; frame < framesRead; ++frame) {
      for (size_t channel = 0; channel < channels; ++channel) {
        float* buffer = (float*)(areas[channel].ptr + areas[channel].step * frame);
        // mix the sample!
        *buffer += chunk[frame * channels + channel];
      }
    }
    delete chunk;
  }
}
