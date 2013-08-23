#ifndef MOON_INPUT_H
#define MOON_INPUT_H

#include "moon.hxx"
#include <GLFW/glfw3.h>
#include <cmath>
#include <array>

#include "mrb.hxx" /* required for friend function */

namespace Moon {

  struct Button {
  public:
    // constructors
    Button();
    Button(int button_id, const char *name);
    // members
    int button_id;
    const char *name;
    int state;
    double held_at; // at what time was the key held down?
    int mods;
  };

  typedef Button KeyboardKey;
  typedef Button MouseButton;

  typedef std::unordered_map<int, Button> ButtonMap;
  typedef ButtonMap KeyMap;

  class Input {
  public:
    class Keyboard {
    public:
      static void update_key(GLFWwindow* window, int key_id, int scancode, int action, int mods);
      static KeyboardKey* get_key(int key_id);
      static int key_mods(int key_id);
      static bool key_is_modded(int key_id, int mod);
      static bool key_state_is_eq(int key_id, int state);
      static bool key_state_is_eq_with_mod(int key_id, int state, int mod);
      static double key_state_hold(int key_id, int state);
    };

    class Mouse {
    public:
      static int x();
      static int y();
      static std::array<int, 2> pos();

      static void update_button(GLFWwindow* window, int button_id, int action, int mods);
      static int button_mods(int button_id);
      static bool button_is_modded(int button_id, int mod);
      static bool button_state_is_eq(int button_id, int state);
      static bool button_state_is_eq_with_mod(int button_id, int state, int mod);
      static double button_state_hold(int button_id, int state);
    };
    // class Joystick { }; // if we ever do this
    // class Gamepad { }; // if we ever do this

    static void initialize(GLFWwindow* window);

  protected:
    static GLFWwindow* window; // the main engine window
    static KeyMap keyboard_keys;
    static ButtonMap mouse_buttons;

  friend void Moon::moon_mrb_input_init(mrb_state*); // give access to map key enums
  };
}

#endif