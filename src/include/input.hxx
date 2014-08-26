#ifndef MOON_INPUT_H
#define MOON_INPUT_H

#include <GLFW/glfw3.h>
#include <cmath>
#include <array>
#include <map>

#include "mrb.hxx"

namespace Moon {
  typedef std::map<int, mrb_value> int__mrb_value_map_t;

  class Input {
  public:
    // Keyboard
    static void on_type(GLFWwindow* window, unsigned int utf8_char);
    static void update_key(GLFWwindow* window, int key_id, int scancode, int action, int mods);
    // Mouse
    static void update_button(GLFWwindow* window, int button_id, int action, int mods);
    static void update_cursor_pos(GLFWwindow* window, double x, double y);

    class Mouse {
    public:
      static int x();
      static int y();
      static std::array<int, 2> get_position();
    };
    // class Joystick { }; // if we ever do this
    // class Gamepad { }; // if we ever do this

    static void initialize(GLFWwindow* window,  mrb_state *mrb);
    static void initialize_maps();

    static mrb_value glfw_state_to_mrb_symbol(int state);
    static mrb_value glfw_key_to_mrb_symbol(int key_id);
  protected:
    static mrb_state* mrb;
    static GLFWwindow* window; // the main engine window
    static int__mrb_value_map_t glfw_state_map;
    static int__mrb_value_map_t glfw_key_map;
  };
};

#endif
