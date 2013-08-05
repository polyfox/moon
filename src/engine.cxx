#include "engine.hxx"

void android_mrb_print_error(mrb_state *mrb);

namespace Moon {
  Engine::Engine() : window(640, 480, "Hello World") {
    #ifdef __ANDROID__
      chdir("/sdcard/moon");
      initialized = false;
    #endif
    #ifndef __ANDROID__
    setup_opengl();

    // setup Input engine
    Input::initialize(window.glfw());

    // setup Gorilla Audio
    Audio::initialize();

    load_mrb();
    #endif
  }

  Engine::initialize() {
    if(!initialized) {
      setup_opengl();
      load_mrb();
      initialized = true;
    }
  }

  Engine::~Engine() { /* Terminate in the reverse order */
    if (mrb) mrb_close(mrb);
    #ifndef __ANDROID__
    Audio::terminate();
    #endif
  }

  void Engine::run() {
    // Get the ruby object containing the state manager
    mrb_value states = mrb_iv_get(mrb, 
                                  mrb_obj_value(mrb_class_get(mrb, "State")), 
                                  mrb_intern(mrb, "@states"));
    
    int ai = mrb_gc_arena_save(mrb);

    while (!window.should_close())
    {
      #ifndef __ANDROID__
      Audio::update();
      #endif

      if (mrb->exc) {
        #if __ANDROID__
          android_mrb_print_error(mrb);
        #else
          mrb_print_error(mrb);
        #endif
        break;
      };

      mrb_funcall(mrb, mrb_funcall(mrb, states, "last", 0), "update", 0);
    #ifdef __ANDROID__
      if(window.can_draw()) {
    #endif
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      mrb_funcall(mrb, mrb_funcall(mrb, states, "last", 0), "render", 0);
    #ifdef __ANDROID__
      }
    #endif

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

    glEnable(GL_BLEND); // Enable blending (alpha transparency)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
    load_mrb_file("./scripts/load.rb");
  }
  
  void Engine::load_core_classes() {
    load_mrb_file("./core/load.rb");
  }

  bool Engine::load_mrb_file(const char *path) {
    FILE *file;

    if(exists(path)) {
      file = fopen(path, "r");
      mrbc_context *cxt = mrbc_context_new(mrb);

      mrbc_filename(mrb, cxt, path);
      mrb_gv_set(mrb, mrb_intern2(mrb, "$0", 2), mrb_str_new_cstr(mrb, path));
      mrb_value v = mrb_load_file_cxt(mrb, file, cxt);

      fclose(file);
      mrbc_context_free(mrb, cxt);

      if(mrb->exc) {
        #if __ANDROID__
          android_mrb_print_error(mrb);
        #else
          mrb_print_error(mrb);
        #endif
        exit(3);
        return false;
      }
    } else {
      std::cout << "file does not exist: " << path << std::endl;
      return false;
    }
    return true;
  }

  int Engine::window_width() {
    return window.width();
  }

  int Engine::window_height() {
    return window.height();
  }

}

/* android */

void
android_mrb_print_backtrace(mrb_state *mrb)
{
#ifdef ENABLE_STDIO
  mrb_callinfo *ci;
  mrb_int ciidx;
  const char *filename, *method, *sep;
  int i, line;

  LOGW("trace:\n", stderr);
  ciidx = mrb_fixnum(mrb_obj_iv_get(mrb, mrb->exc, mrb_intern2(mrb, "ciidx", 5)));
  if (ciidx >= mrb->c->ciend - mrb->c->cibase)
    ciidx = 10; /* ciidx is broken... */

  for (i = ciidx; i >= 0; i--) {
    ci = &mrb->c->cibase[i];
    filename = "(unknown)";
    line = -1;

    if (MRB_PROC_CFUNC_P(ci->proc)) {
      continue;
    }
    else {
      mrb_irep *irep = ci->proc->body.irep;
      if (irep->filename != NULL)
        filename = irep->filename;
      if (irep->lines != NULL) {
        mrb_code *pc;

        if (i+1 <= ciidx) {
          pc = mrb->c->cibase[i+1].pc;
        }
        else {
          pc = (mrb_code*)mrb_voidp(mrb_obj_iv_get(mrb, mrb->exc, mrb_intern2(mrb, "lastpc", 6)));
        }
        if (irep->iseq <= pc && pc < irep->iseq + irep->ilen) {
          line = irep->lines[pc - irep->iseq - 1];
        }
      }
    }
    if (line == -1) continue;
    if (ci->target_class == ci->proc->target_class)
      sep = ".";
    else
      sep = "#";

    method = mrb_sym2name(mrb, ci->mid);
    if (method) {
      const char *cn = mrb_class_name(mrb, ci->proc->target_class);

      if (cn) {
        LOGW("\t[%d] %s:%d:in %s%s%s\n", i, filename, line, cn, sep, method);
      }
      else {
        LOGW("\t[%d] %s:%d:in %s\n", i, filename, line, method);
      }
    }
    else {
      LOGW("\t[%d] %s:%d\n", i, filename, line);
    }
  }
#endif
}

void
android_mrb_print_error(mrb_state *mrb)
{
#ifdef ENABLE_STDIO
  mrb_value s;
  
  mrb_print_backtrace(mrb);
  s = mrb_funcall(mrb, mrb_obj_value(mrb->exc), "inspect", 0);
  if (mrb_string_p(s)) {
    struct RString *str = mrb_str_ptr(s);
    std::string error(str->ptr, str->len);
    LOGW("%s\n", error.c_str());
  }
#endif
}