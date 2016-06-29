#ifndef MOON_AUDIO_GORILLA_H
#define MOON_AUDIO_GORILLA_H

/* Gorilla Audio */
#include <gorilla/ga.h>
#include <gorilla/gau.h>
#define MOON_AUDIO_BACKEND "gorilla"

namespace Moon {
  class Audio {
  public:
    static bool Initialize();
    static void Update();
    static void Terminate();
    static ga_Mixer* GetMixer();
    static ga_StreamManager* GetStreamMgr();
  protected:
    static bool m_initialized;
    static gau_Manager* m_mgr;
    static ga_Mixer* m_mixer;
    static ga_StreamManager* m_streamMgr;
  };
};

#endif
