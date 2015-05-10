#ifndef MOON_FPS_H
#define MOON_FPS_H

namespace Moon {
  class FPS {
  public:
    FPS();

    int   GetFPS();
    float GetDelta();
    void  Update();
    float Restart();
  private:
    float m_old_time;
    float m_last_time;
    float m_delta; // time since last frame
    int   m_fps;
    int   m_frames;
  };
};

#endif
