#ifndef MOON_MUSIC_H
#define MOON_MUSIC_H

#include "moon.hxx"
#include "audio.hxx"
namespace Moon {
  class Music : public Audio {
  public:
    Music(std::string filename);
    ~Music();
  private:

  };
};

#endif