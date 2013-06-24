#ifndef MOON_ENGINE_H
#define MOON_ENGINE_H

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include "unistd.h"
#include "dirent.h"
#include <string.h>

/* OpenGL */
#include <GL/gl.h>
#include <GLFW/glfw3.h>

/* Include the mruby header */
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/compile.h>
#include <mruby/variable.h>
#include <mruby/string.h>

void moon_init_mrb_ext(mrb_state *mrb);

namespace Moon {
  class Engine {
  public:
    Engine();
    ~Engine();
    void run();

  private:
    //EngineSetup *setup;
    mrb_state *mrb;
    mrbc_context *mrb_context;
    //mrb_value game_object;
    
    void load_mrb();
    void load_mrb_file(const char*, const char*);
    void load_mrb_files_from_dir(const char*);
    void load_core_classes();
    void load_user_scripts();
  };
};
#endif