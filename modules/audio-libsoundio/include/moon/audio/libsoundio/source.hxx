#ifndef MOON_AUDIO_LIBSOUNDIO_SOURCE_H
#define MOON_AUDIO_LIBSOUNDIO_SOURCE_H

#include "moon/intern.h"

namespace Moon
{
  class Source {
    public:
      virtual int read(float* dst, int frames) = 0;
      // seek(pos, type);
      //
      virtual int channels() = 0;
      virtual int sampleRate() = 0;
  };
}

#endif
