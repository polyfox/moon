#ifndef MOON_AUDIO_LIBSOUNDIO_MUSIC_H
#define MOON_AUDIO_LIBSOUNDIO_MUSIC_H

#include "moon/intern.h"
#include <string>
#include <sndfile.hh>
#include "moon/audio/libsoundio/source.hxx"
#include "moon/audio/libsoundio/mixer.hxx"

namespace Moon
{
  struct LoopSettings {
    bool enabled;
    unsigned int start;
    unsigned int end;

    LoopSettings() :
      enabled(false),
      start(0),
      end(0) {}
  };

  class Music : public Source {
    public:
      Music(const std::string filename);
      virtual ~Music();

      int read(float* dst, int frames);

      int length();
      int channels();
      int sampleRate();
      void setLoop(unsigned int loopStart, unsigned int loopEnd);
      void clearLoop();
    private:
      SndfileHandle m_file;
      LoopSettings m_loop;
  };
};

#endif
