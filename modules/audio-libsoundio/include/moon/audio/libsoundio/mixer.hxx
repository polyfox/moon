#ifndef MOON_AUDIO_LIBSOUNDIO_MIXER_H
#define MOON_AUDIO_LIBSOUNDIO_MIXER_H

#include "moon/intern.h"
#include <soundio/soundio.h>
#include <vector>
#include "moon/audio/libsoundio/handle.hxx"

namespace Moon
{
  class Mixer {
    public:
      Mixer();

      void mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames);
      std::vector<Handle*> handles;
    private:
  };
}

#endif
