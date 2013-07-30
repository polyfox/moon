#ifndef MOON_ENGINE_H
#define MOON_ENGINE_H

#include "moon.hxx"

#include "input.hxx"
#include "texture.hxx"
#include "sprite.hxx"
#include "spritesheet.hxx"
#include "fps.hxx"
#include "audio.hxx"
#include "music.hxx"
#include "sound.hxx"

void moon_init_mrb_core(mrb_state *mrb); // core side
void moon_init_mrb_ext(mrb_state *mrb);

namespace Moon {
  class Engine {
  public:
    Engine();
    ~Engine();
    void run();

  private:
    mrb_state *mrb;

    GLFWwindow*  window; // GLFW window

    void load_mrb();
    bool load_mrb_file(const char*, const char*);
    void load_core_classes();
    void load_user_scripts();

    void setup_glfw();
    void setup_opengl();
  };
};
#endif