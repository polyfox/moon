#ifndef MOON_AUDIO_LIBSOUNDIO_SOUND_H
#define MOON_AUDIO_LIBSOUNDIO_SOUND_H

#include "moon/intern.h"
#include <string>
#include <sndfile.hh>
#include "moon/audio/libsoundio/source.hxx"
#include "moon/audio/libsoundio/mixer.hxx"

namespace Moon
{
  class Sound : public Source {
    public:
      Sound(const std::string filename);
      ~Sound();

      int read(float* dst, int frames);

      int channels();
      int sampleRate();
    private:
      float* source; // [] ?
      int m_channels;
      int m_sampleRate;

      std::uint32_t totalFrames;
      std::uint32_t currentFrame;
  };
};

#endif
