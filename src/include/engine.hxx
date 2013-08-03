#ifndef MOON_ENGINE_H
#define MOON_ENGINE_H

#include "moon.hxx"

#include "window.hxx"
#ifndef __ANDROID__
#include "input.hxx"
#endif
#include "texture.hxx"
#include "sprite.hxx"
#include "spritesheet.hxx"
#ifndef __ANDROID__
#include "audio.hxx"
#include "music.hxx"
#include "sound.hxx"
#endif

void moon_init_mrb_core(mrb_state *mrb); // core side
void moon_init_mrb_ext(mrb_state *mrb);

void handle_cmd(struct android_app* app, int32_t cmd);

namespace Moon {
  class Engine {
  public:
    Engine();
    ~Engine();
    void run();

    int window_width();
    int window_height();
    
  private:
    mrb_state *mrb;

    Window window;

    void load_mrb();
    bool load_mrb_file(const char* path);
    void load_core_classes();
    void load_user_scripts();

    void setup_opengl();
  #ifdef __ANDROID__
  friend void ::android_main(struct android_app* state);
  friend void ::handle_cmd(struct android_app* app, int32_t cmd);
  #endif
  };
};

int moon_main(Moon::Engine *engine); // user entry point

#endif