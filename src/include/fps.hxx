#ifndef MOON_FPS_H
#define MOON_FPS_H

#include <GLFW/glfw3.h>

namespace Moon {
  class FPS {
  public:
    FPS();

    void  update();
    int   getFPS();
    float getDelta();
    float getSpeedFactor();
  private:
    float OldTime;
    float LastTime;

    float Delta; // time since last frame

    float SpeedFactor;

    int   NumFrames;
    int   Frames;

  };
};

#endif