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

    bool load_mrb_file(const char *path);
    //void set_resource_path(const char *path);
  private:
    mrb_state *mrb;
    FPS fps;
    Window window;

    std::string resource_path;

    void load_mrb();

    void setup_opengl();
  };
};

void moon_load_user_scripts(Moon::Engine *engine);
void moon_config(Moon::Engine *engine);
int moon_main(Moon::Engine *engine); // user entry point

#endif