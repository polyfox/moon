#ifndef MOON_ENGINE_H
#define MOON_ENGINE_H

#include "moon.hxx"

#include "window.hxx"
#include "input.hxx"
#include "audio.hxx"
#include "fps.hxx"

#include "mrb.hxx"

namespace Moon {
  class Engine {
  public:
    Engine();
    ~Engine();
    void run();
    
  private:
    mrb_state *mrb;
    FPS fps;
    Window window;

    void load_mrb();
    bool load_mrb_file(const char* path);

    void setup_opengl();
  };
};

int moon_main(Moon::Engine *engine); // user entry point

#endif