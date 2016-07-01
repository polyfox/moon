#ifndef MOON_AUDIO_LIBSOUNDIO_SOURCE_H
#define MOON_AUDIO_LIBSOUNDIO_SOURCE_H

#include "moon/intern.h"
#include <string>
#include <sndfile.h>

namespace Moon
{
  class Source {
    public:
      SF_INFO info;
      SNDFILE* file;

      Source(const std::string filename);
      ~Source();

      bool seekable();
      sf_count_t seek(sf_count_t frames, int whence);
      sf_count_t tell();
      sf_count_t read(float* ptr, sf_count_t frames);
  };
}

#endif
