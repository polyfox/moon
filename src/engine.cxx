#include "moon/engine.hxx"
#include "moon/mrb/engine.hxx"
#include "moon/mrb/window.hxx"
#include <clocale>

namespace Moon {
  bool LEGACY_GL = false;

  Engine::Engine() :
    window(800, 600, "Hello World"),
    resource_path("resources")
  {
    std::setlocale(LC_ALL, "en_US.UTF-8"); // set locale to UTF-8

    moon_config(this);

    setup_opengl();

    // setup Gorilla Audio
    Audio::initialize();

    load_mrb();

    // setup Input engine
    Input::initialize(window.glfw(), mrb);
  }

  Engine::~Engine() { /* Terminate in the reverse order */
    if (mrb) mrb_close(mrb);

    Audio::terminate();
  }

  void Engine::run() {
    int ai = mrb_gc_arena_save(mrb);

    while (!window.should_close())
    {
      Audio::update();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      if (mrb->exc) {
        mrb_print_error(mrb);
        break;
      };

      moon_step(mrb, fps.getDelta());
      mrb_gc_arena_restore(mrb, ai);

      fps.update();
      window.update(&fps);
    }
  }

  void Engine::setup_opengl() {
    glDisable(GL_DITHER);

    glEnable(GL_BLEND); // Enable blending (alpha transparency)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Check for error
    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
      printf("Error initializing OpenGL! glGetError: %i\n", error);
      throw;
    }

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
      // GLEW failed!
      exit(1);
    }
  }

  void Engine::load_mrb() {
    mrb = mrb_open();

    mmrb_init(mrb);

    mrb_value engine_val = mrb_obj_value(Data_Wrap_Struct(mrb, mrb->object_class, &engine_data_type, (void*)this));
    mrb_mod_cv_set(mrb, mmrb_Moon, mrb_intern_cstr(mrb, "engine"), engine_val);

    mrb_value window_val = mrb_obj_value(Data_Wrap_Struct(mrb, mrb->object_class, &window_data_type, (void*)(&window)));
    mrb_mod_cv_set(mrb, mmrb_Moon, mrb_intern_cstr(mrb, "window"), window_val);

    load_mrb_file("./core/load.rb"); // load core classes
    moon_load_user_scripts(this);
  }

  bool Engine::load_mrb_file(const char *path) {
    FILE *file;

    if(exists(path)) {
      file = fopen(path, "r");
      mrbc_context *cxt = mrbc_context_new(mrb);

      mrbc_filename(mrb, cxt, path);
      mrb_gv_set(mrb, mrb_intern_cstr(mrb, "$0"), mrb_str_new_cstr(mrb, path));

      mrb_load_file_cxt(mrb, file, cxt);

      fclose(file);
      mrbc_context_free(mrb, cxt);

      if(mrb->exc) {
        mrb_print_error(mrb);
        exit(3);
        return false;
      }
    } else {
      printf("Error loading mrb file: file does not exist %s", path);
      return false;
    }
    return true;
  }

  //void Engine::set_resource_path(const char *path) {
  //  resource_path = std::string(path);
  //}

}
