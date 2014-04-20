#ifndef MOON_WINDOW_H
#define MOON_WINDOW_H

#include "moon.hxx"
#include "fps.hxx"
#include "shader.hxx" // window.resize()
#include <GLFW/glfw3.h>

namespace Moon {
  class Window {
  public:
    Window(int width, int height, const char* title);
    ~Window();

    GLFWwindow* glfw(); // TEMPORARY HACK
    void update();
    void resize(int width, int height);
    bool should_close();
    int width();
    int height();
  private:
    FPS fps;
    GLFWwindow* window; // GLFW window
    int window_width;
    int window_height;
  };
};

#endif