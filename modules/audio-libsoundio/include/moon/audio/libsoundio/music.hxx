#ifndef MOON_AUDIO_LIBSOUNDIO_MUSIC_H
#define MOON_AUDIO_LIBSOUNDIO_MUSIC_H

#include "moon/intern.h"
#include <string>
#include <sndfile.hh>
#include "moon/audio/libsoundio/source.hxx"
#include "moon/audio/libsoundio/mixer.hxx"

namespace Moon
{
  class Music : public Source {
    public:
      Music(const std::string filename);
      virtual ~Music();

      int read(float* dst, int frames);
      std::uint32_t seek(std::uint32_t pos);

      int channels();
      int sampleRate();
    private:
      SndfileHandle file;
  };
};

#endif
