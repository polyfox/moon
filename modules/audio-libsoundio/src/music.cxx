#include "moon/intern.h"
#include "moon/audio/libsoundio/music.hxx"

namespace Moon
{
  Music::Music(const std::string filename) :
    file(filename, SFM_READ)
  {
    printf("Opened file '%s'\n", filename.c_str());
    printf("    Sample rate : %d\n", file.samplerate());
    printf("    Channels    : %d\n", file.channels());
  };


  Music::~Music() {
  }

  int Music::channels() {
    return file.channels();
  }

  int Music::sampleRate() {
    return file.samplerate();
  }

  // returns how many frames we actually read
  int Music::read(float* dst, int frames)
  {
    return file.readf(dst, frames);
  }
}
