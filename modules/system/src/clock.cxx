#include "moon/time.hxx"
#include "moon/clock.hxx"

namespace Moon {
  Clock::Clock() {
    m_old_time  = 0;
    m_last_time = 0;
    m_delta     = 0;
    m_frames    = 0;
    m_fps       = 0;
  }

  uint64_t Clock::GetDelta() {
    return m_delta;
  };

  int Clock::GetFPS() {
    return m_fps;
  }

  void Clock::Update() {
    uint64_t now = moon_get_current_time();
    if ((m_old_time + 1000000) < now) {
      m_old_time = now;
      m_fps = m_frames;
      m_frames = 0;
    }
    m_delta = now - m_last_time;
    m_last_time = now;
    m_frames++;
  }

  uint64_t Clock::Restart() {
    uint64_t d = GetDelta();
    Update();
    return d;
  };
}
