#ifndef MOON_INPUT_H
#define MOON_INPUT_H

#include "moon.hxx"

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
    static void initialize();
    static void update_key(GLFWwindow *, int key, int scancode, int action, int mods);
    static KeyboardKey* get_key(int);
    static bool key_pressed(int);
    static bool key_released(int);
  protected:  
    static KeyMap keyboard_keys;
  };
}

#endif