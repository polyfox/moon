#include "engine.hxx"
/* DevIL */
#include <IL/il.h>
#include <IL/ilu.h>

namespace Moon {
  Engine::Engine() {
    setup_glfw();
    setup_opengl();

    load_mrb();
  }

  Engine::~Engine() {
    glfwTerminate();
    mrbc_context_free(mrb, mrb_context);
    mrb_close(mrb);
  }

  void Engine::run() {
    // Get the ruby object containing the state manager
    mrb_value states = mrb_iv_get(mrb, 
                                  mrb_obj_value(mrb_class_get(mrb, "State")), 
                                  mrb_intern(mrb, "@states"));
    
    int ai = mrb_gc_arena_save(mrb);

    while (!glfwWindowShouldClose(window))
    {
      FPS::FPSControl.onLoop();
      char title[50];
      sprintf(title, "FPS: %i", FPS::FPSControl.getFPS());
      glfwSetWindowTitle(window, title);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      if (mrb->exc) {
        mrb_print_error(mrb);
        break;
      };

      mrb_funcall(mrb, mrb_funcall(mrb, states, "last", 0), "update", 0);
      mrb_gc_arena_restore(mrb, ai);

      glfwSwapBuffers(window);
      glfwPollEvents(); /* Poll for and process events */
    }

  }
  void Engine::setup_glfw() {
    if (!glfwInit()) {
      printf( "Error initializing glfw!");
      throw;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // Use OpenGL Core v2.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    glfwMakeContextCurrent(window);
    std::cout << "OpenGL v" << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR) << "." << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR) << std::endl;
  }

  void Engine::setup_opengl() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, viewport[2], viewport[3], 0, -1, 1); // Sets up OpenGL so that (0,0) corresponds to the top left corner, and (640,480) corresponds to the bottom right corner.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.375, 0.375, 0.0); // http://www.opengl.org/archives/resources/faq/technical/transformations.htm#tran0030

    glDisable(GL_LIGHTING);
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
    // UPDATE: depth test can be used, but alpha testing needs to be enabled too,
    // or the alpha background will be black.
    glEnable (GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);

    glEnable(GL_TEXTURE_2D); // Use 2D textures

    //Check for error
    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
      printf( "Error initializing OpenGL! glGetError: %i\n", error);
      throw;
    }

    //Initialize DevIL
    ilInit();
    ilClearColour(255, 255, 255, 000);

    //Check for error
    ILenum ilError = ilGetError();
    if(ilError != IL_NO_ERROR) {
      printf("Error initializing DevIL! %s\n", iluErrorString(ilError));
      throw;
    }
    // end initialize DevIL
  }

  void Engine::load_mrb() {
    mrb = mrb_open();
    mrb_context = mrbc_context_new(mrb); // debugger context

    moon_init_mrb_core(mrb);
    moon_init_mrb_ext(mrb);

    load_core_classes();
    load_user_scripts();
    mrbc_filename(mrb, mrb_context, "main");
  }

  void Engine::load_user_scripts() {
    load_mrb_files_from_dir("./script/");
  }
  
  void Engine::load_core_classes() {
    load_mrb_files_from_dir("./core/");
  }
  
  bool Engine::load_mrb_files_from_dir(const char *directory) {
    DIR *dir;
    struct dirent *ent;
    dir = opendir(directory);
    
    if (dir != NULL) {
      while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, ".rb")) {
          bool success = load_mrb_file(directory, ent->d_name);
          if (!success) return false;
        } else if (ent->d_type == DT_DIR) {
          if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
            std::string path;
            path += directory;
            path += ent->d_name;
            path += "/";
            load_mrb_files_from_dir(path.c_str());
          }
        }
      }
    }
    return true;
  }
  
  bool Engine::load_mrb_file(const char *file_path, const char *filename) {
    char path[1024];

    mrb_sym zero_sym = mrb_intern2(mrb, "$0", 2);

    strcpy(path, file_path);
    strcat(path, filename);

    FILE *file;

    file = fopen((const char*)path, "r");
    mrbc_filename(mrb, mrb_context, filename);
    mrb_gv_set(mrb, zero_sym, mrb_str_new_cstr(mrb, filename));
    mrb_value v = mrb_load_file_cxt(mrb, file, mrb_context);

    fclose(file);
    if(mrb->exc) {
      if (!mrb_undef_p(v)) {
        mrb_print_error(mrb);
      }
      exit(312);
      return false;
    } else {
      std::cout << "script: " << path << std::endl;
    }
    return true;
  }
}