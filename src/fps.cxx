#include "fps.hxx"

namespace Moon {
  FPS FPS::FPSControl;

  FPS::FPS() {
    OldTime     = 0.0f;
    LastTime    = 0.0f;
    SpeedFactor = 0;
    Frames      = 0;
    NumFrames   = 0;
  }

  void FPS::onLoop() {
    if(OldTime + 1.0f < glfwGetTime()) {
      OldTime = glfwGetTime();
      NumFrames = Frames;
      Frames = 0;
    }
    SpeedFactor = ((glfwGetTime() - LastTime) / 1.0f) * 32.0f; // 32 is number of pixels per second
    LastTime = glfwGetTime();
    Frames++;
  }

  int FPS::getFPS() {
    return NumFrames;
  }

  float FPS::getSpeedFactor() {
    return SpeedFactor;
  }
}