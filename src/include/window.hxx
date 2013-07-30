#ifndef MOON_WINDOW_H
#define MOON_WINDOW_H

#include "moon.hxx"
#include "fps.hxx"
#include <GLFW/glfw3.h>

namespace Moon {
  class Window {
  public:
    Window(int width, int height, const char* title);
    ~Window();

    GLFWwindow* glfw(); // TEMPORARY HACK
    void update();
    bool should_close();
  private:
    FPS fps;
    GLFWwindow* window; // GLFW window
  };
};

#endif