#ifndef MOON_SOUND_H
#define MOON_SOUND_H

#include "moon.hxx"
#include "audio.hxx"

namespace Moon {
  class Sound : public Audio {
  public:
    Sound(std::string filename, std::string format);
    ~Sound();

    bool play();
    bool play(float gain, float pitch, float pan);
  private:
    ga_Sound* sound;
  };
};

#endif