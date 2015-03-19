#ifndef MOON_FPS_H
#define MOON_FPS_H

namespace Moon {
  class FPS {
  public:
    FPS();

    int   getFPS();
    float getDelta();
    void  update();
    float restart();
  private:
    float old_time;
    float last_time;
    float delta; // time since last frame
    int   fps;
    int   frames;
  };
};

#endif
