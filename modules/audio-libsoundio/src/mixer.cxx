#include "moon/audio/libsoundio/mixer.hxx"

namespace Moon {
  Mixer::Mixer()  {
  }

  void Mixer::mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames)
  {
    for(auto const& handle: handles) { 
      handle->mix(areas, layout, sampleRate, frames);
    }
  }
}
