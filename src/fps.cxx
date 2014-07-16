#include "fps.hxx"

namespace Moon {
  FPS::FPS() {
    old_time  = 0.0;
    last_time = 0.0;
    delta     = 0.0;
    frames    = 0;
    fps       = 0;
  }

  void FPS::update() {
    float now = glfwGetTime();
    if(old_time + 1.0f < now) {
      old_time = now;
      fps = frames;
      frames = 0;
    }
    delta = now - last_time;
    last_time = now;
    frames++;
  }

  float FPS::getDelta() {
    return delta;
  };

  int FPS::getFPS() {
    return fps;
  }
}