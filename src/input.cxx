#include "input.hxx"

namespace Moon {

  Button::Button() {
    button_id = 0;
    state     = GLFW_RELEASE;
    name      = NULL;
    held_at   = 0;
    mods      = 0;
  }

  Button::Button(int glfw_key, const char* kname) {
    button_id = glfw_key;
    state     = GLFW_RELEASE;
    name      = kname;
    held_at   = 0;
    mods      = 0;
  }

  KeyMap Input::keyboard_keys = {
    { GLFW_KEY_SPACE, { GLFW_KEY_SPACE, "SPACE" } },
    { GLFW_KEY_APOSTROPHE, { GLFW_KEY_APOSTROPHE, "APOSTROPHE" } },
    { GLFW_KEY_COMMA, { GLFW_KEY_COMMA, "COMMA" } },
    { GLFW_KEY_MINUS, { GLFW_KEY_MINUS, "MINUS" } },
    { GLFW_KEY_PERIOD, { GLFW_KEY_PERIOD, "PERIOD" } },
    { GLFW_KEY_SLASH, { GLFW_KEY_SLASH, "SLASH" } },
    { GLFW_KEY_0, { GLFW_KEY_0, "N0" } }, /* cannot define ruby numeric constant, so we prefix N */
    { GLFW_KEY_1, { GLFW_KEY_1, "N1" } },
    { GLFW_KEY_2, { GLFW_KEY_2, "N2" } },
    { GLFW_KEY_3, { GLFW_KEY_3, "N3" } },
    { GLFW_KEY_4, { GLFW_KEY_4, "N4" } },
    { GLFW_KEY_5, { GLFW_KEY_5, "N5" } },
    { GLFW_KEY_6, { GLFW_KEY_6, "N6" } },
    { GLFW_KEY_7, { GLFW_KEY_7, "N7" } },
    { GLFW_KEY_8, { GLFW_KEY_8, "N8" } },
    { GLFW_KEY_9, { GLFW_KEY_9, "N9" } },
    { GLFW_KEY_SEMICOLON, { GLFW_KEY_SEMICOLON, "SEMICOLON" } },
    { GLFW_KEY_EQUAL, { GLFW_KEY_EQUAL, "EQUAL" } },
    { GLFW_KEY_A, { GLFW_KEY_A, "A" } },
    { GLFW_KEY_B, { GLFW_KEY_B, "B" } },
    { GLFW_KEY_C, { GLFW_KEY_C, "C" } },
    { GLFW_KEY_D, { GLFW_KEY_D, "D" } },
    { GLFW_KEY_E, { GLFW_KEY_E, "E" } },
    { GLFW_KEY_F, { GLFW_KEY_F, "F" } },
    { GLFW_KEY_G, { GLFW_KEY_G, "G" } },
    { GLFW_KEY_H, { GLFW_KEY_H, "H" } },
    { GLFW_KEY_I, { GLFW_KEY_I, "I" } },
    { GLFW_KEY_J, { GLFW_KEY_J, "J" } },
    { GLFW_KEY_K, { GLFW_KEY_K, "K" } },
    { GLFW_KEY_L, { GLFW_KEY_L, "L" } },
    { GLFW_KEY_M, { GLFW_KEY_M, "M" } },
    { GLFW_KEY_N, { GLFW_KEY_N, "N" } },
    { GLFW_KEY_O, { GLFW_KEY_O, "O" } },
    { GLFW_KEY_P, { GLFW_KEY_P, "P" } },
    { GLFW_KEY_Q, { GLFW_KEY_Q, "Q" } },
    { GLFW_KEY_R, { GLFW_KEY_R, "R" } },
    { GLFW_KEY_S, { GLFW_KEY_S, "S" } },
    { GLFW_KEY_T, { GLFW_KEY_T, "T" } },
    { GLFW_KEY_U, { GLFW_KEY_U, "U" } },
    { GLFW_KEY_V, { GLFW_KEY_V, "V" } },
    { GLFW_KEY_W, { GLFW_KEY_W, "W" } },
    { GLFW_KEY_X, { GLFW_KEY_X, "X" } },
    { GLFW_KEY_Y, { GLFW_KEY_Y, "Y" } },
    { GLFW_KEY_Z, { GLFW_KEY_Z, "Z" } },
    { GLFW_KEY_LEFT_BRACKET, { GLFW_KEY_LEFT_BRACKET, "LEFT_BRACKET" } },
    { GLFW_KEY_BACKSLASH, { GLFW_KEY_BACKSLASH, "BACKSLASH" } },
    { GLFW_KEY_RIGHT_BRACKET, { GLFW_KEY_RIGHT_BRACKET, "RIGHT_BRACKET" } },
    { GLFW_KEY_GRAVE_ACCENT, { GLFW_KEY_GRAVE_ACCENT, "GRAVE_ACCENT" } },
    { GLFW_KEY_WORLD_1, { GLFW_KEY_WORLD_1, "WORLD_1" } },
    { GLFW_KEY_WORLD_2, { GLFW_KEY_WORLD_2, "WORLD_2" } },
    { GLFW_KEY_ESCAPE, { GLFW_KEY_ESCAPE, "ESCAPE" } },
    { GLFW_KEY_ENTER, { GLFW_KEY_ENTER, "ENTER" } },
    { GLFW_KEY_TAB, { GLFW_KEY_TAB, "TAB" } },
    { GLFW_KEY_BACKSPACE, { GLFW_KEY_BACKSPACE, "BACKSPACE" } },
    { GLFW_KEY_INSERT, { GLFW_KEY_INSERT, "INSERT" } },
    { GLFW_KEY_DELETE, { GLFW_KEY_DELETE, "DELETE" } },
    { GLFW_KEY_RIGHT, { GLFW_KEY_RIGHT, "RIGHT" } },
    { GLFW_KEY_LEFT, { GLFW_KEY_LEFT, "LEFT" } },
    { GLFW_KEY_DOWN, { GLFW_KEY_DOWN, "DOWN" } },
    { GLFW_KEY_UP, { GLFW_KEY_UP, "UP" } },
    { GLFW_KEY_PAGE_UP, { GLFW_KEY_PAGE_UP, "PAGE_UP" } },
    { GLFW_KEY_PAGE_DOWN, { GLFW_KEY_PAGE_DOWN, "PAGE_DOWN" } },
    { GLFW_KEY_HOME, { GLFW_KEY_HOME, "HOME" } },
    { GLFW_KEY_END, { GLFW_KEY_END, "END" } },
    { GLFW_KEY_CAPS_LOCK, { GLFW_KEY_CAPS_LOCK, "CAPS_LOCK" } },
    { GLFW_KEY_SCROLL_LOCK, { GLFW_KEY_SCROLL_LOCK, "SCROLL_LOCK" } },
    { GLFW_KEY_NUM_LOCK, { GLFW_KEY_NUM_LOCK, "NUM_LOCK" } },
    { GLFW_KEY_PRINT_SCREEN, { GLFW_KEY_PRINT_SCREEN, "PRINT_SCREEN" } },
    { GLFW_KEY_PAUSE, { GLFW_KEY_PAUSE, "PAUSE" } },
    { GLFW_KEY_F1, { GLFW_KEY_F1, "F1" } },
    { GLFW_KEY_F2, { GLFW_KEY_F2, "F2" } },
    { GLFW_KEY_F3, { GLFW_KEY_F3, "F3" } },
    { GLFW_KEY_F4, { GLFW_KEY_F4, "F4" } },
    { GLFW_KEY_F5, { GLFW_KEY_F5, "F5" } },
    { GLFW_KEY_F6, { GLFW_KEY_F6, "F6" } },
    { GLFW_KEY_F7, { GLFW_KEY_F7, "F7" } },
    { GLFW_KEY_F8, { GLFW_KEY_F8, "F8" } },
    { GLFW_KEY_F9, { GLFW_KEY_F9, "F9" } },
    { GLFW_KEY_F10, { GLFW_KEY_F10, "F10" } },
    { GLFW_KEY_F11, { GLFW_KEY_F11, "F11" } },
    { GLFW_KEY_F12, { GLFW_KEY_F12, "F12" } },
    { GLFW_KEY_F13, { GLFW_KEY_F13, "F13" } },
    { GLFW_KEY_F14, { GLFW_KEY_F14, "F14" } },
    { GLFW_KEY_F15, { GLFW_KEY_F15, "F15" } },
    { GLFW_KEY_F16, { GLFW_KEY_F16, "F16" } },
    { GLFW_KEY_F17, { GLFW_KEY_F17, "F17" } },
    { GLFW_KEY_F18, { GLFW_KEY_F18, "F18" } },
    { GLFW_KEY_F19, { GLFW_KEY_F19, "F19" } },
    { GLFW_KEY_F20, { GLFW_KEY_F20, "F20" } },
    { GLFW_KEY_F21, { GLFW_KEY_F21, "F21" } },
    { GLFW_KEY_F22, { GLFW_KEY_F22, "F22" } },
    { GLFW_KEY_F23, { GLFW_KEY_F23, "F23" } },
    { GLFW_KEY_F24, { GLFW_KEY_F24, "F24" } },
    { GLFW_KEY_F25, { GLFW_KEY_F25, "F25" } },
    { GLFW_KEY_KP_0, { GLFW_KEY_KP_0, "KP_0" } },
    { GLFW_KEY_KP_1, { GLFW_KEY_KP_1, "KP_1" } },
    { GLFW_KEY_KP_2, { GLFW_KEY_KP_2, "KP_2" } },
    { GLFW_KEY_KP_3, { GLFW_KEY_KP_3, "KP_3" } },
    { GLFW_KEY_KP_4, { GLFW_KEY_KP_4, "KP_4" } },
    { GLFW_KEY_KP_5, { GLFW_KEY_KP_5, "KP_5" } },
    { GLFW_KEY_KP_6, { GLFW_KEY_KP_6, "KP_6" } },
    { GLFW_KEY_KP_7, { GLFW_KEY_KP_7, "KP_7" } },
    { GLFW_KEY_KP_8, { GLFW_KEY_KP_8, "KP_8" } },
    { GLFW_KEY_KP_9, { GLFW_KEY_KP_9, "KP_9" } },
    { GLFW_KEY_KP_DECIMAL, { GLFW_KEY_KP_DECIMAL, "KP_DECIMAL" } },
    { GLFW_KEY_KP_DIVIDE, { GLFW_KEY_KP_DIVIDE, "KP_DIVIDE" } },
    { GLFW_KEY_KP_MULTIPLY, { GLFW_KEY_KP_MULTIPLY, "KP_MULTIPLY" } },
    { GLFW_KEY_KP_SUBTRACT, { GLFW_KEY_KP_SUBTRACT, "KP_SUBTRACT" } },
    { GLFW_KEY_KP_ADD, { GLFW_KEY_KP_ADD, "KP_ADD" } },
    { GLFW_KEY_KP_ENTER, { GLFW_KEY_KP_ENTER, "KP_ENTER" } },
    { GLFW_KEY_KP_EQUAL, { GLFW_KEY_KP_EQUAL, "KP_EQUAL" } },
    { GLFW_KEY_LEFT_SHIFT, { GLFW_KEY_LEFT_SHIFT, "LEFT_SHIFT" } },
    { GLFW_KEY_LEFT_CONTROL, { GLFW_KEY_LEFT_CONTROL, "LEFT_CONTROL" } },
    { GLFW_KEY_LEFT_ALT, { GLFW_KEY_LEFT_ALT, "LEFT_ALT" } },
    { GLFW_KEY_LEFT_SUPER, { GLFW_KEY_LEFT_SUPER, "LEFT_SUPER" } },
    { GLFW_KEY_RIGHT_SHIFT, { GLFW_KEY_RIGHT_SHIFT, "RIGHT_SHIFT" } },
    { GLFW_KEY_RIGHT_CONTROL, { GLFW_KEY_RIGHT_CONTROL, "RIGHT_CONTROL" } },
    { GLFW_KEY_RIGHT_ALT, { GLFW_KEY_RIGHT_ALT, "RIGHT_ALT" } },
    { GLFW_KEY_RIGHT_SUPER, { GLFW_KEY_RIGHT_SUPER, "RIGHT_SUPER" } },
    { GLFW_KEY_MENU, { GLFW_KEY_MENU, "MENU" } }
  };

  ButtonMap Input::mouse_buttons = {
    { GLFW_MOUSE_BUTTON_1, { GLFW_MOUSE_BUTTON_1, "BUTTON_1" } }, // LEFT
    { GLFW_MOUSE_BUTTON_2, { GLFW_MOUSE_BUTTON_2, "BUTTON_2" } }, // RIGHT
    { GLFW_MOUSE_BUTTON_3, { GLFW_MOUSE_BUTTON_3, "BUTTON_3" } }, // MIDDLE
    { GLFW_MOUSE_BUTTON_4, { GLFW_MOUSE_BUTTON_4, "BUTTON_4" } },
    { GLFW_MOUSE_BUTTON_5, { GLFW_MOUSE_BUTTON_5, "BUTTON_5" } },
    { GLFW_MOUSE_BUTTON_6, { GLFW_MOUSE_BUTTON_6, "BUTTON_6" } },
    { GLFW_MOUSE_BUTTON_7, { GLFW_MOUSE_BUTTON_7, "BUTTON_7" } },
    { GLFW_MOUSE_BUTTON_8, { GLFW_MOUSE_BUTTON_8, "BUTTON_8" } }
  };

  GLFWwindow* Input::window = NULL;

  void Input::initialize(GLFWwindow* glfw_window) {
    window = glfw_window;
    glfwSetKeyCallback(window, Input::Keyboard::update_key);
    glfwSetMouseButtonCallback(window, Input::Mouse::update_button);
  }

  void Input::Keyboard::update_key(GLFWwindow* window,
                         int key_id, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
      keyboard_keys[key_id].held_at = glfwGetTime();
    } else if (action == GLFW_RELEASE) {
      keyboard_keys[key_id].held_at = 0;
    }

    keyboard_keys[key_id].state = action;
    keyboard_keys[key_id].mods = mods;
  }

  KeyboardKey* Input::Keyboard::get_key(int key_id) {
    return &keyboard_keys[key_id];
  }

  int Input::Keyboard::key_mods(int key_id) {
    return keyboard_keys[key_id].mods;
  }

  bool Input::Keyboard::key_is_modded(int key_id, int mod) {
    return (keyboard_keys[key_id].mods & mod) == mod;
  }

  bool Input::Keyboard::key_state_is_eq(int key_id, int state) {
    return keyboard_keys[key_id].state == state;
  }

  bool Input::Keyboard::key_state_is_eq_with_mod(int key_id, int state, int mod) {
    return Input::Keyboard::key_state_is_eq(key_id, state) && Input::Keyboard::key_is_modded(key_id, mod);
  }

  double Input::Keyboard::key_state_hold(int key_id, int state) {
    if (mouse_buttons[key_id].held_at != 0) {
      return glfwGetTime() - mouse_buttons[key_id].held_at;
    } else {
      return 0;
    }
  }

  /* Mouse functions */

  void Input::Mouse::update_button(GLFWwindow* window,
                                   int button_id, int action, int mods) {
    if (action == GLFW_PRESS) {
      mouse_buttons[button_id].held_at = glfwGetTime();
    } else if (action == GLFW_RELEASE) {
      mouse_buttons[button_id].held_at = 0;
    }

    mouse_buttons[button_id].state = action;
    mouse_buttons[button_id].mods = mods;
  }

  int Input::Mouse::button_mods(int button_id) {
    return mouse_buttons[button_id].mods;
  }

  bool Input::Mouse::button_is_modded(int button_id, int mod) {
    return (mouse_buttons[button_id].mods & mod) == mod;
  }

  bool Input::Mouse::button_state_is_eq(int button_id, int state) {
    return mouse_buttons[button_id].state == state;
  }

  bool Input::Mouse::button_state_is_eq_with_mod(int button_id, int state,
                                                 int mod) {
    return Input::Mouse::button_state_is_eq(button_id, state) &&
           Input::Mouse::button_is_modded(button_id, mod);
  }

  double Input::Mouse::button_state_hold(int button_id, int state) {
    if (mouse_buttons[button_id].held_at != 0) {
      return glfwGetTime() - mouse_buttons[button_id].held_at;
    } else {
      return 0;
    }
  }

  int Input::Mouse::x() {
    double x;
    glfwGetCursorPos(window, &x, NULL);
    return floor(x);
  }

  int Input::Mouse::y() {
    double y;
    glfwGetCursorPos(window, NULL, &y);
    return floor(y);
  }

  std::array<int, 2> Input::Mouse::pos() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return {(int)floor(x), (int)floor(y)};
  }
}