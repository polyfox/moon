#include "moon/intern.h"
#include "moon/audio/libsoundio/music.hxx"

namespace Moon
{
  Music::Music(const std::string filename) :
    file(filename, SFM_READ)
  {
  };


  Music::~Music() {
  }


  void Music::mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames) {
    // TODO: compare file.channels() with layout.channel_count
    // handle mono to stereo, vice versa
    for (int frame = 0; frame < frames; ++frame) {
      for (size_t channel = 0; channel < file.channels(); ++channel) {
        float* buffer = (float*)(areas[channel].ptr + areas[channel].step * frame);
        float sample = 0.0f;

        // read here, write to sample
        file.read(&sample, 1);

        // clipping, to avoid overdrive
        *buffer = sample > 1.0f ? 1.0f : (sample < -1.0f ? -1.0f : sample);
      }
    }
  }
}
