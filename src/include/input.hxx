#ifndef MOON_INPUT_H
#define MOON_INPUT_H

#include <GLFW/glfw3.h>
#include <cmath>
#include <array>

#include "mrb.hxx"

namespace Moon {

  class Input {
  public:
    // Keyboard
    static void update_key(GLFWwindow* window, int key_id, int scancode, int action, int mods);
    // Mouse
    static void update_button(GLFWwindow* window, int button_id, int action, int mods);

    class Mouse {
    public:
      static int x();
      static int y();
      static std::array<int, 2> pos();
    };
    // class Joystick { }; // if we ever do this
    // class Gamepad { }; // if we ever do this

    static void initialize(GLFWwindow* window,  mrb_state *mrb);

  protected:
    static mrb_state* mrb;
    static GLFWwindow* window; // the main engine window
  };
};

#endif