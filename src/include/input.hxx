#ifndef MOON_INPUT_H
#define MOON_INPUT_H

#include "moon.hxx"
#include <cmath>

namespace Moon {

  struct Button {
  public:
    Button();
    Button(int button_id, char* name);
    int button_id;
    char* name;
    int state;
    int mods;  
  };

  typedef Button KeyboardKey;
  typedef Button MouseButton;

  typedef std::unordered_map<int, Button> ButtonMap;
  typedef ButtonMap KeyMap;

  class Input {
  public: 
    static void initialize(GLFWwindow* window);
    static void update_key(GLFWwindow* window, int key_id, int scancode, int action, int mods);
    static KeyboardKey* get_key(int key_id);
    static bool key_pressed(int key_index);
    static bool key_released(int key_index);
    static bool key_repeated(int key_index);

    class Mouse {
    public:
      static int x();
      static int y();
      static void update_button(GLFWwindow* window, int button_id, int action, int mods);
      static bool button_pressed(int button_index);
      static bool button_released(int button_index);
    };

  protected:
    static GLFWwindow* window; // the main engine window
    static KeyMap keyboard_keys;
    static ButtonMap mouse_buttons;

  friend void Moon::moon_mrb_input_init(mrb_state*); // give access to map key enums
  };
}

#endif