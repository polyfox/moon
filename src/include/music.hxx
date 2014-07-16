#ifndef MOON_MUSIC_H
#define MOON_MUSIC_H

#include "audio.hxx"
namespace Moon {
  class Music : public Audio {
  public:
    Music(std::string filename, std::string format);
    ~Music();
    bool setup_loop(int trigger, int target);
    bool clear_loop();
    bool play(float gain, float pitch, float pan);
    void stop();
    bool is_playing();
    bool is_stopped();
    bool is_finished();
    bool seek(int offset);
    int pos();
    int length();
  private:
    ga_Handle* handle;
    gau_SampleSourceLoop* loopSrc;
  };
};

#endif
