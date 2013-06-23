#include "engine.hxx"

namespace Moon {

  Engine::Engine() {
    glfwInit(); // TODO: check if lib was inited properly
    load_mrb();
  }

  Engine::~Engine() {
    std::cout << "exterminated." << std::endl;
    glfwTerminate();
    mrb_close(mrb);
    mrbc_context_free(mrb, mrb_context);
  }

  void Engine::run() {
    GLFWwindow* window;
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    glfwMakeContextCurrent(window);
    //load_game_object();
    while (!glfwWindowShouldClose(window))
    {

      float ratio;
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);
      ratio = width / (float) height;
      glViewport(0, 0, width, height);
      glClear(GL_COLOR_BUFFER_BIT);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
      glBegin(GL_TRIANGLES);
      glColor3f(1.f, 0.f, 0.f);
      glVertex3f(-0.6f, -0.4f, 0.f);
      glColor3f(0.f, 1.f, 0.f);
      glVertex3f(0.6f, -0.4f, 0.f);
      glColor3f(0.f, 0.f, 1.f);
      glVertex3f(0.f, 0.6f, 0.f);
      glEnd();

      glfwSwapBuffers(window);
      /* Poll for and process events */
      glfwPollEvents();
    }

    //mrb_funcall(mrb, game_object, "update", 0);
  }

  void Engine::load_mrb() {
    mrb = mrb_open();
    // mrb_context: Debugger context
    mrb_context = mrbc_context_new(mrb);
    mrb_context->capture_errors = 0;
    mrb_context->dump_result = 0;
    mrb_context->no_exec = 0;
    
    load_core_classes();
    load_user_scripts();
  }

  void Engine::load_user_scripts() {
    load_mrb_files_from_dir("./script/");
  }
  
  void Engine::load_core_classes() {
    load_mrb_files_from_dir("./core/");
  }
  
  void Engine::load_mrb_files_from_dir(const char *directory) {
    DIR *dir;
    struct dirent *ent;
    dir = opendir(directory);
    
    if (dir != NULL) {
      while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, ".rb")) {
          load_mrb_file(directory, ent->d_name);
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
  }
  
  void Engine::load_mrb_file(const char *file_path, const char *filename) {
    char path[1024];
    
    strcpy(path, file_path);
    strcat(path, filename);
    
    FILE *file;
    
    file = fopen((const char*)path, "r");
    mrb_load_file_cxt(mrb, file, mrb_context);
    std::cout << path << " loaded." << std::endl;
    fclose(file);
  }
}