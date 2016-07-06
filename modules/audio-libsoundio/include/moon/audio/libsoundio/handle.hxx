#ifndef MOON_AUDIO_LIBSOUNDIO_HANDLE_H
#define MOON_AUDIO_LIBSOUNDIO_HANDLE_H

#include "moon/intern.h"
#include <vector>
#include <soundio/soundio.h>
#include "moon/audio/libsoundio/source.hxx"
#include "moon/audio/libsoundio/music.hxx"
#include "moon/audio/libsoundio/sound.hxx"

namespace Moon {
  class Handle {
    public:
      Handle(Moon::Source* source);
      ~Handle();

      void mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames);
    private:
      Moon::Source* source; // TODO:mrb_sound/music sources will need to be wrapped... if a source deallocates, the handle will be broken (shared_ptr)
  };
};

#endif
