#ifndef MOON_AUDIO_LIBSOUNDIO_SOUND_H
#define MOON_AUDIO_LIBSOUNDIO_SOUND_H

#include "moon/intern.h"
#include <string>
#include <sndfile.hh>
#include "moon/audio/libsoundio/mixer.hxx"

namespace Moon
{
  class Sound : public AbstractSource {
    public:
      Sound(const std::string filename);
      ~Sound();

      void mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames);
    private:
      float* source; // [] ?
      int channels;
      int sampleRate;

      int totalFrames;
      int currentFrame;
  };
};

#endif
