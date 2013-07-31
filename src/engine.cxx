#include "engine.hxx"

namespace Moon {
  Engine::Engine() : window(640, 480, "Hello World") {
    setup_opengl();

    // setup Input engine
    Input::initialize(window.glfw());

    // setup Gorilla Audio
    Audio::initialize();

    load_mrb();
  }

  Engine::~Engine() { /* Terminate in the reverse order */
    if (mrb) mrb_close(mrb);

    Audio::terminate();
  }

  void Engine::run() {
    // Get the ruby object containing the state manager
    mrb_value states = mrb_iv_get(mrb, 
                                  mrb_obj_value(mrb_class_get(mrb, "State")), 
                                  mrb_intern(mrb, "@states"));
    
    int ai = mrb_gc_arena_save(mrb);

    while (!window.should_close())
    {
      Audio::update();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      if (mrb->exc) {
        mrb_print_error(mrb);
        break;
      };

      mrb_funcall(mrb, mrb_funcall(mrb, states, "last", 0), "update", 0);
      mrb_funcall(mrb, mrb_funcall(mrb, states, "last", 0), "render", 0);
      mrb_gc_arena_restore(mrb, ai);

      window.update();
    }
  }

  void Engine::setup_opengl() {
    // Sets up the projection matrix so that (0,0) corresponds to the top left corner, and (640,480) corresponds to the bottom right corner.
    GLfloat viewport[4];
    glGetFloatv(GL_VIEWPORT, viewport);
    Shader::projection_matrix = glm::ortho(0.f, viewport[2], viewport[3], 0.f, -1.f, 1.f);

    glDisable(GL_DITHER);
    // Enable blending (alpha transparency)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Depth testing is useful in 3D OpenGL applications where if you render
    // something and then render something that's behind it, the object that's
    // behind won't raster it's polygons over the object that's in front because
    // the depth is tested first. The reason we disable depth testing is when you
    // mix blending and depth testing you get funky results.
    // -------------------------------------------------------------------------
    // UPDATE: Depth test can be used, but alpha testing needs to be enabled too,
    // or the alpha background will be black.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);

    //Check for error
    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
      printf( "Error initializing OpenGL! glGetError: %i\n", error);
      throw;
    }
  }

  void Engine::load_mrb() {
    mrb = mrb_open();
  
    moon_init_mrb_core(mrb);
    moon_init_mrb_ext(mrb);
    
    load_core_classes();
    load_user_scripts();
  }

  void Engine::load_user_scripts() {
    load_mrb_file("./scripts/", "load.rb");
  }
  
  void Engine::load_core_classes() {
    load_mrb_file("./core/", "load.rb");
  }

  bool Engine::load_mrb_file(const char *file_path, const char *filename) {
    char path[1024];
    FILE *file;

    mrb_sym zero_sym = mrb_intern2(mrb, "$0", 2);

    strcpy(path, file_path);
    strcat(path, filename);

    if(exists(path)) {
      file = fopen((const char*)path, "r");
      mrbc_context *cxt = mrbc_context_new(mrb);

      mrbc_filename(mrb, cxt, filename);
      mrb_gv_set(mrb, zero_sym, mrb_str_new_cstr(mrb, filename));
      mrb_value v = mrb_load_file_cxt(mrb, file, cxt);

      fclose(file);
      mrbc_context_free(mrb, cxt);

      if(mrb->exc) {
        if (!mrb_undef_p(v)) {
          mrb_print_error(mrb);
        }
        exit(312);
        return false;
      } else {
        std::cout << "script: " << path << std::endl;
      }
    } else {
      std::cout << "file does not exist: " << path << std::endl;
    }
    return true;
  }
}