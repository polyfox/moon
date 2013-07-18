#include "input.hxx"

namespace Moon {

  KeyboardKey::KeyboardKey() {
    key = 0;
    state = GLFW_RELEASE;
    name = NULL;
    mods = 0;
  }

  KeyboardKey::KeyboardKey(int glfw_key, char* kname) {
    key = glfw_key;
    state = GLFW_RELEASE;
    name = kname;
    mods = 0;
  }

  /* appended K to all key names, for easy ruby constantizing */
  KeyMap Input::keyboard_keys = {
    { GLFW_KEY_SPACE, { GLFW_KEY_SPACE, "KSPACE" } },
    { GLFW_KEY_APOSTROPHE, { GLFW_KEY_APOSTROPHE, "KAPOSTROPHE" } },
    { GLFW_KEY_COMMA, { GLFW_KEY_COMMA, "KCOMMA" } },
    { GLFW_KEY_MINUS, { GLFW_KEY_MINUS, "KMINUS" } },
    { GLFW_KEY_PERIOD, { GLFW_KEY_PERIOD, "KPERIOD" } },
    { GLFW_KEY_SLASH, { GLFW_KEY_SLASH, "KSLASH" } },
    { GLFW_KEY_0, { GLFW_KEY_0, "K0" } },
    { GLFW_KEY_1, { GLFW_KEY_1, "K1" } },
    { GLFW_KEY_2, { GLFW_KEY_2, "K2" } },
    { GLFW_KEY_3, { GLFW_KEY_3, "K3" } },
    { GLFW_KEY_4, { GLFW_KEY_4, "K4" } },
    { GLFW_KEY_5, { GLFW_KEY_5, "K5" } },
    { GLFW_KEY_6, { GLFW_KEY_6, "K6" } },
    { GLFW_KEY_7, { GLFW_KEY_7, "K7" } },
    { GLFW_KEY_8, { GLFW_KEY_8, "K8" } },
    { GLFW_KEY_9, { GLFW_KEY_9, "K9" } },
    { GLFW_KEY_SEMICOLON, { GLFW_KEY_SEMICOLON, "KSEMICOLON" } },
    { GLFW_KEY_EQUAL, { GLFW_KEY_EQUAL, "KEQUAL" } },
    { GLFW_KEY_A, { GLFW_KEY_A, "KA" } },
    { GLFW_KEY_B, { GLFW_KEY_B, "KB" } },
    { GLFW_KEY_C, { GLFW_KEY_C, "KC" } },
    { GLFW_KEY_D, { GLFW_KEY_D, "KD" } },
    { GLFW_KEY_E, { GLFW_KEY_E, "KE" } },
    { GLFW_KEY_F, { GLFW_KEY_F, "KF" } },
    { GLFW_KEY_G, { GLFW_KEY_G, "KG" } },
    { GLFW_KEY_H, { GLFW_KEY_H, "KH" } },
    { GLFW_KEY_I, { GLFW_KEY_I, "KI" } },
    { GLFW_KEY_J, { GLFW_KEY_J, "KJ" } },
    { GLFW_KEY_K, { GLFW_KEY_K, "KK" } },
    { GLFW_KEY_L, { GLFW_KEY_L, "KL" } },
    { GLFW_KEY_M, { GLFW_KEY_M, "KM" } },
    { GLFW_KEY_N, { GLFW_KEY_N, "KN" } },
    { GLFW_KEY_O, { GLFW_KEY_O, "KO" } },
    { GLFW_KEY_P, { GLFW_KEY_P, "KP" } },
    { GLFW_KEY_Q, { GLFW_KEY_Q, "KQ" } },
    { GLFW_KEY_R, { GLFW_KEY_R, "KR" } },
    { GLFW_KEY_S, { GLFW_KEY_S, "KS" } },
    { GLFW_KEY_T, { GLFW_KEY_T, "KT" } },
    { GLFW_KEY_U, { GLFW_KEY_U, "KU" } },
    { GLFW_KEY_V, { GLFW_KEY_V, "KV" } },
    { GLFW_KEY_W, { GLFW_KEY_W, "KW" } },
    { GLFW_KEY_X, { GLFW_KEY_X, "KX" } },
    { GLFW_KEY_Y, { GLFW_KEY_Y, "KY" } },
    { GLFW_KEY_Z, { GLFW_KEY_Z, "KZ" } },
    { GLFW_KEY_LEFT_BRACKET, { GLFW_KEY_LEFT_BRACKET, "KLEFT_BRACKET" } },
    { GLFW_KEY_BACKSLASH, { GLFW_KEY_BACKSLASH, "KBACKSLASH" } },
    { GLFW_KEY_RIGHT_BRACKET, { GLFW_KEY_RIGHT_BRACKET, "KRIGHT_BRACKET" } },
    { GLFW_KEY_GRAVE_ACCENT, { GLFW_KEY_GRAVE_ACCENT, "KGRAVE_ACCENT" } },
    { GLFW_KEY_WORLD_1, { GLFW_KEY_WORLD_1, "KWORLD_1" } },
    { GLFW_KEY_WORLD_2, { GLFW_KEY_WORLD_2, "KWORLD_2" } },
    { GLFW_KEY_ESCAPE, { GLFW_KEY_ESCAPE, "KESCAPE" } },
    { GLFW_KEY_ENTER, { GLFW_KEY_ENTER, "KENTER" } },
    { GLFW_KEY_TAB, { GLFW_KEY_TAB, "KTAB" } },
    { GLFW_KEY_BACKSPACE, { GLFW_KEY_BACKSPACE, "KBACKSPACE" } },
    { GLFW_KEY_INSERT, { GLFW_KEY_INSERT, "KINSERT" } },
    { GLFW_KEY_DELETE, { GLFW_KEY_DELETE, "KDELETE" } },
    { GLFW_KEY_RIGHT, { GLFW_KEY_RIGHT, "KRIGHT" } },
    { GLFW_KEY_LEFT, { GLFW_KEY_LEFT, "KLEFT" } },
    { GLFW_KEY_DOWN, { GLFW_KEY_DOWN, "KDOWN" } },
    { GLFW_KEY_UP, { GLFW_KEY_UP, "KUP" } },
    { GLFW_KEY_PAGE_UP, { GLFW_KEY_PAGE_UP, "KPAGE_UP" } },
    { GLFW_KEY_PAGE_DOWN, { GLFW_KEY_PAGE_DOWN, "KPAGE_DOWN" } },
    { GLFW_KEY_HOME, { GLFW_KEY_HOME, "KHOME" } },
    { GLFW_KEY_END, { GLFW_KEY_END, "KEND" } },
    { GLFW_KEY_CAPS_LOCK, { GLFW_KEY_CAPS_LOCK, "KCAPS_LOCK" } },
    { GLFW_KEY_SCROLL_LOCK, { GLFW_KEY_SCROLL_LOCK, "KSCROLL_LOCK" } },
    { GLFW_KEY_NUM_LOCK, { GLFW_KEY_NUM_LOCK, "KNUM_LOCK" } },
    { GLFW_KEY_PRINT_SCREEN, { GLFW_KEY_PRINT_SCREEN, "KPRINT_SCREEN" } },
    { GLFW_KEY_PAUSE, { GLFW_KEY_PAUSE, "KPAUSE" } },
    { GLFW_KEY_F1, { GLFW_KEY_F1, "KF1" } },
    { GLFW_KEY_F2, { GLFW_KEY_F2, "KF2" } },
    { GLFW_KEY_F3, { GLFW_KEY_F3, "KF3" } },
    { GLFW_KEY_F4, { GLFW_KEY_F4, "KF4" } },
    { GLFW_KEY_F5, { GLFW_KEY_F5, "KF5" } },
    { GLFW_KEY_F6, { GLFW_KEY_F6, "KF6" } },
    { GLFW_KEY_F7, { GLFW_KEY_F7, "KF7" } },
    { GLFW_KEY_F8, { GLFW_KEY_F8, "KF8" } },
    { GLFW_KEY_F9, { GLFW_KEY_F9, "KF9" } },
    { GLFW_KEY_F10, { GLFW_KEY_F10, "KF10" } },
    { GLFW_KEY_F11, { GLFW_KEY_F11, "KF11" } },
    { GLFW_KEY_F12, { GLFW_KEY_F12, "KF12" } },
    { GLFW_KEY_F13, { GLFW_KEY_F13, "KF13" } },
    { GLFW_KEY_F14, { GLFW_KEY_F14, "KF14" } },
    { GLFW_KEY_F15, { GLFW_KEY_F15, "KF15" } },
    { GLFW_KEY_F16, { GLFW_KEY_F16, "KF16" } },
    { GLFW_KEY_F17, { GLFW_KEY_F17, "KF17" } },
    { GLFW_KEY_F18, { GLFW_KEY_F18, "KF18" } },
    { GLFW_KEY_F19, { GLFW_KEY_F19, "KF19" } },
    { GLFW_KEY_F20, { GLFW_KEY_F20, "KF20" } },
    { GLFW_KEY_F21, { GLFW_KEY_F21, "KF21" } },
    { GLFW_KEY_F22, { GLFW_KEY_F22, "KF22" } },
    { GLFW_KEY_F23, { GLFW_KEY_F23, "KF23" } },
    { GLFW_KEY_F24, { GLFW_KEY_F24, "KF24" } },
    { GLFW_KEY_F25, { GLFW_KEY_F25, "KF25" } },
    { GLFW_KEY_KP_0, { GLFW_KEY_KP_0, "KKP_0" } },
    { GLFW_KEY_KP_1, { GLFW_KEY_KP_1, "KKP_1" } },
    { GLFW_KEY_KP_2, { GLFW_KEY_KP_2, "KKP_2" } },
    { GLFW_KEY_KP_3, { GLFW_KEY_KP_3, "KKP_3" } },
    { GLFW_KEY_KP_4, { GLFW_KEY_KP_4, "KKP_4" } },
    { GLFW_KEY_KP_5, { GLFW_KEY_KP_5, "KKP_5" } },
    { GLFW_KEY_KP_6, { GLFW_KEY_KP_6, "KKP_6" } },
    { GLFW_KEY_KP_7, { GLFW_KEY_KP_7, "KKP_7" } },
    { GLFW_KEY_KP_8, { GLFW_KEY_KP_8, "KKP_8" } },
    { GLFW_KEY_KP_9, { GLFW_KEY_KP_9, "KKP_9" } },
    { GLFW_KEY_KP_DECIMAL, { GLFW_KEY_KP_DECIMAL, "KKP_DECIMAL" } },
    { GLFW_KEY_KP_DIVIDE, { GLFW_KEY_KP_DIVIDE, "KKP_DIVIDE" } },
    { GLFW_KEY_KP_MULTIPLY, { GLFW_KEY_KP_MULTIPLY, "KKP_MULTIPLY" } },
    { GLFW_KEY_KP_SUBTRACT, { GLFW_KEY_KP_SUBTRACT, "KKP_SUBTRACT" } },
    { GLFW_KEY_KP_ADD, { GLFW_KEY_KP_ADD, "KKP_ADD" } },
    { GLFW_KEY_KP_ENTER, { GLFW_KEY_KP_ENTER, "KKP_ENTER" } },
    { GLFW_KEY_KP_EQUAL, { GLFW_KEY_KP_EQUAL, "KKP_EQUAL" } },
    { GLFW_KEY_LEFT_SHIFT, { GLFW_KEY_LEFT_SHIFT, "KLEFT_SHIFT" } },
    { GLFW_KEY_LEFT_CONTROL, { GLFW_KEY_LEFT_CONTROL, "KLEFT_CONTROL" } },
    { GLFW_KEY_LEFT_ALT, { GLFW_KEY_LEFT_ALT, "KLEFT_ALT" } },
    { GLFW_KEY_LEFT_SUPER, { GLFW_KEY_LEFT_SUPER, "KLEFT_SUPER" } },
    { GLFW_KEY_RIGHT_SHIFT, { GLFW_KEY_RIGHT_SHIFT, "KRIGHT_SHIFT" } },
    { GLFW_KEY_RIGHT_CONTROL, { GLFW_KEY_RIGHT_CONTROL, "KRIGHT_CONTROL" } },
    { GLFW_KEY_RIGHT_ALT, { GLFW_KEY_RIGHT_ALT, "KRIGHT_ALT" } },
    { GLFW_KEY_RIGHT_SUPER, { GLFW_KEY_RIGHT_SUPER, "KRIGHT_SUPER" } },
    { GLFW_KEY_MENU, { GLFW_KEY_MENU, "KMENU" } }
  };
  
  void Input::initialize() {
    //
  }

  KeyboardKey* Input::get_key(int key_id) {
    return &keyboard_keys[key_id];
  }

  bool Input::key_pressed(int key_index) {
    return keyboard_keys[key_index].state == GLFW_PRESS;
  }

  bool Input::key_released(int key_index) {
    return keyboard_keys[key_index].state == GLFW_RELEASE;
  }

  void Input::update_key(GLFWwindow* window, 
                         int key, int scancode, int action, int mods) {
    keyboard_keys[key].state = action;
    keyboard_keys[key].mods = mods;
  }

}