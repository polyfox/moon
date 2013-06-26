#ifndef MOON_FPS_H
#define MOON_FPS_H

#include "moon.hxx"

namespace Moon {
  class FPS {
  public:
    static FPS FPSControl;
    FPS();

    void    onLoop();
    int     getFPS();
    float   getSpeedFactor();
  private:
    float OldTime;
    float LastTime;

    float SpeedFactor;

    int   NumFrames;
    int   Frames;

  };
};

#endif