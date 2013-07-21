#ifndef MOON_MUSIC_H
#define MOON_MUSIC_H

#include "moon.hxx"
#include "audio.hxx"
namespace Moon {
  class Music : public Audio {
  public:
    static bool play(std::string filename, std::string format);
    static void stop();
    static bool is_playing();
    static bool is_stopped();
    static bool is_finished();
    static bool seek(int offset);
    static int pos();
    static int length();
  private:
    static ga_Handle* handle;
  };
};

#endif