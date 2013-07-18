#ifndef MOON_INPUT_H
#define MOON_INPUT_H

#include "moon.hxx"

namespace Moon {
  typedef struct moon_keyboard_key {
    int key;
    int state;
    char* name;
  } KeyboardKey;

  class Input {
  public: 
    static void init();
    static void update(GLFWwindow*);
    static void add_key(int, int, char*);
    static KeyboardKey* get_key(int);
    static bool key_pressed(int);
    static bool key_released(int);
  protected:  
    static KeyboardKey keyboard_keys[120];
  };
}

#endif