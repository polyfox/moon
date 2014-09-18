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
  private:
    float old_time;
    float last_time;
    float delta; // time since last frame
    int   fps;
    int   frames;
  };
};

#endif