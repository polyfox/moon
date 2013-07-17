#ifndef MOON_MUSIC_H
#define MOON_MUSIC_H

#include "moon.hxx"
#include "audio.hxx"
namespace Moon {
  class Music : public Audio {
  public:
    static void play(std::string filename, std::string format);
    static void stop();
  private:
    static ga_Handle* handle;
  };
};

#endif