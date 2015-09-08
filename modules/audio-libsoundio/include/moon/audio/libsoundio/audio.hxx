#ifndef MOON_AUDIO_H
#define MOON_AUDIO_H

#include <soundio/soundio.h>

namespace Moon {
  class Audio {
    public:
      static bool Initialize();
      static void Update();
      static void Terminate();
    protected:
      static bool m_initialized;
      static struct SoundIo *m_soundIO;
      static struct SoundIoDevice *m_device;
      static struct SoundIoOutStream *m_outStream;
  };
};

#endif
