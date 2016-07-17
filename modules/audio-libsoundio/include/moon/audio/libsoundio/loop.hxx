#ifndef MOON_AUDIO_LIBSOUNDIO_LOOP_H
#define MOON_AUDIO_LIBSOUNDIO_LOOP_H

#include "moon/intern.h"
#include <string>
#include <sndfile.hh>
#include "moon/audio/libsoundio/source.hxx"
#include "moon/audio/libsoundio/mixer.hxx"

namespace Moon
{
  class Loop : public Source {
    public:
      Loop(Moon::Source* source, std::uint32_t trigger, std::uint32_t target);
      virtual ~Loop();

      int read(float* dst, int frames);
      std::uint32_t seek(std::uint32_t pos);

      int channels();
      int sampleRate();
    private:
      std::uint32_t trigger;
      std::uint32_t target;
      Moon::Source* source; // TODO:mrb_sound/music sources will need to be wrapped... if a source deallocates, the handle will be broken (shared_ptr)
  };
};

#endif
