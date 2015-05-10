#include <GLFW/glfw3.h>
#include "moon/fps.hxx"

namespace Moon {
  FPS::FPS() {
    m_old_time  = 0.0;
    m_last_time = 0.0;
    m_delta     = 0.0;
    m_frames    = 0;
    m_fps       = 0;
  }

  float FPS::GetDelta() {
    return m_delta;
  };

  int FPS::GetFPS() {
    return m_fps;
  }

  void FPS::Update() {
    float now = glfwGetTime();
    if ((m_old_time + 1.0f) < now) {
      m_old_time = now;
      m_fps = m_frames;
      m_frames = 0;
    }
    m_delta = now - m_last_time;
    m_last_time = now;
    m_frames++;
  }

  float FPS::Restart() {
    float d = GetDelta();
    Update();
    return d;
  };
}
