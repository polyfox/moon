#include "moon/window.hxx"

namespace Moon {
  Window::Window(int width, int height, const char* title) {
    if (!glfwInit()) {
      printf( "Error initializing glfw!");
      throw;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // Use OpenGL Core v3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for 3.0
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // for 3.0 and on

    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if(!window) { // 3.3 wasn't created, let's try 2.1
      LEGACY_GL = true;
      glfwDefaultWindowHints();
      glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
      window = glfwCreateWindow(width, height, title, NULL, NULL);
    }

    window_width = width;
    window_height = height;
    glfwMakeContextCurrent(window);
    std::cout << "OpenGL v" << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR) << "." << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR) << std::endl;

    resize(width, height); // trigger setting up the projection matrix
  };

  Window::~Window() {
    glfwTerminate();
  };

  void Window::update(FPS *fps) {
    char title[50];
    sprintf(title, "FPS: %i", fps->getFPS());
    glfwSetWindowTitle(window, title);

    glfwSwapBuffers(window);
    glfwPollEvents(); /* Poll for and process events */
  };

  void Window::resize(int width, int height) {
    glfwSetWindowSize(window, width, height);

    // Sets up the projection matrix so that (0,0) corresponds to the top left corner,
    // and (width, height) corresponds to the bottom right corner.
    Shader::projection_matrix = glm::ortho(0.f, (float)width, (float)height, 0.f, -1.f, 1.f);
  }

  bool Window::should_close() {
    return glfwWindowShouldClose(window);
  }

  int Window::width() {
    return window_width;
  }

  int Window::height() {
    return window_height;
  }

  GLFWwindow* Window::glfw() {
    return window;
  };
};
