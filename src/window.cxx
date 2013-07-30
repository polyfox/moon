#include "window.hxx"

namespace Moon {
  Window::Window(int width, int height, const char* title) {
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

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwMakeContextCurrent(window);
    std::cout << "OpenGL v" << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR) << "." << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR) << std::endl;
  };

  Window::~Window() {
    glfwTerminate();
  };

  void Window::update() {
    fps.update();
    char title[50];
    sprintf(title, "FPS: %i", fps.getFPS());
    glfwSetWindowTitle(window, title);

    glfwSwapBuffers(window);
    glfwPollEvents(); /* Poll for and process events */
  };

  bool Window::should_close() {
    glfwWindowShouldClose(window);
  }

  GLFWwindow* Window::glfw() {
    return window;
  };
};
