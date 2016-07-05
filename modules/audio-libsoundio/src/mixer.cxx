#include "moon/audio/libsoundio/mixer.hxx"

namespace Moon {
  Mixer::Mixer()  {
  }

  void Mixer::mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames)
  {
    // silence the buffer first
    for (size_t channel = 0; channel < layout.channel_count; ++channel) {
      memset(areas[channel].ptr, 0, areas[channel].step * frames);
    }
    for(auto const& handle: handles) { 
      handle->mix(areas, layout, sampleRate, frames);
    }

    // clipping, to avoid overdrive
    for (int frame = 0; frame < frames; ++frame) {
      for (size_t channel = 0; channel < channels; ++channel) {
        float* buffer = (float*)(areas[channel].ptr + areas[channel].step * frame);
        *buffer = sample > 1.0f ? 1.0f : (sample < -1.0f ? -1.0f : sample);
      }
    }
  }
}
