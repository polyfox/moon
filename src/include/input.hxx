#ifndef MOON_INPUT_H
#define MOON_INPUT_H

#include "moon.hxx"
#include <cmath>

namespace Moon {

  struct KeyboardKey {
  public:
    KeyboardKey();
    KeyboardKey(int, char*);
    int key;
    char* name;
    int state;
    int mods;
  };
 
  typedef std::unordered_map<int, KeyboardKey> KeyMap;

  class Input {
  public: 
    static void initialize(GLFWwindow* window);
    static void update_key(GLFWwindow* window, int key, int scancode, int action, int mods);
    static KeyboardKey* get_key(int key_id);
    static bool key_pressed(int key_index);
    static bool key_released(int key_index);
    // TODO: handle GLFW_REPEAT which triggers after a second or so

    class Mouse {
    public:
      static int x();
      static int y();
    };
  protected:
    static GLFWwindow* window; // the main engine window
    static KeyMap keyboard_keys;

  friend void Moon::moon_mrb_input_init(mrb_state*); // give access to map key enums
  };
}

#endif