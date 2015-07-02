#ifndef MOON_CLOCK_H
#define MOON_CLOCK_H

#include <stdint.h>

namespace Moon {
  class Clock {
  public:
    Clock();

    int      GetFPS();
    uint64_t GetDelta();
    void     Update();
    uint64_t Restart();
  private:
    uint64_t m_old_time;
    uint64_t m_last_time;
    uint64_t m_delta; // time since last frame
    int m_fps;
    int m_frames;
  };
};

#endif
