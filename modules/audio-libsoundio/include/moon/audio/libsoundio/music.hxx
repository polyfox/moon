#ifndef MOON_AUDIO_LIBSOUNDIO_MUSIC_H
#define MOON_AUDIO_LIBSOUNDIO_MUSIC_H

#include "moon/intern.h"
#include <string>
#include <sndfile.hh>
#include "moon/audio/libsoundio/mixer.hxx"

namespace Moon
{
  class Music : public AbstractSource {
    public:
      Music(const std::string filename);
      ~Music();

      void mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames);
    private:
      SndfileHandle file;
  };
};

#endif
