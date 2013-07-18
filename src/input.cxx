#include "input.hxx"

namespace Moon {

  void Input::add_key(int index, int glfw_key, char* name) {  
    KeyboardKey *key = &Input::keyboard_keys[index];
    key->key = glfw_key;
    key->state = -1;
    key->name = name;
  }
  
  void Input::initialize() {
    /* append K to all key names, for easy to ruby constantizing */
    Input::add_key(  0, GLFW_KEY_SPACE, "KSPACE");
    Input::add_key(  1, GLFW_KEY_APOSTROPHE , "KAPOSTROPHE");
    Input::add_key(  2, GLFW_KEY_COMMA , "KCOMMA");
    Input::add_key(  3, GLFW_KEY_MINUS , "KMINUS");
    Input::add_key(  4, GLFW_KEY_PERIOD, "KPERIOD");
    Input::add_key(  5, GLFW_KEY_SLASH , "KSLASH");
    Input::add_key(  6, GLFW_KEY_0, "K0");
    Input::add_key(  7, GLFW_KEY_1, "K1");
    Input::add_key(  8, GLFW_KEY_2, "K2");
    Input::add_key(  9, GLFW_KEY_3, "K3");
    Input::add_key( 10, GLFW_KEY_4, "K4");
    Input::add_key( 11, GLFW_KEY_5, "K5");
    Input::add_key( 12, GLFW_KEY_6, "K6");
    Input::add_key( 13, GLFW_KEY_7, "K7");
    Input::add_key( 14, GLFW_KEY_8, "K8");
    Input::add_key( 15, GLFW_KEY_9, "K9");
    Input::add_key( 16, GLFW_KEY_SEMICOLON , "KSEMICOLON");
    Input::add_key( 17, GLFW_KEY_EQUAL , "KEQUAL");
    Input::add_key( 18, GLFW_KEY_A, "KA");
    Input::add_key( 19, GLFW_KEY_B, "KB");
    Input::add_key( 20, GLFW_KEY_C, "KC");
    Input::add_key( 21, GLFW_KEY_D, "KD");
    Input::add_key( 22, GLFW_KEY_E, "KE");
    Input::add_key( 23, GLFW_KEY_F, "KF");
    Input::add_key( 24, GLFW_KEY_G, "KG");
    Input::add_key( 25, GLFW_KEY_H, "KH");
    Input::add_key( 26, GLFW_KEY_I, "KI");
    Input::add_key( 27, GLFW_KEY_J, "KJ");
    Input::add_key( 28, GLFW_KEY_K, "KK");
    Input::add_key( 29, GLFW_KEY_L, "KL");
    Input::add_key( 30, GLFW_KEY_M, "KM");
    Input::add_key( 31, GLFW_KEY_N, "KN");
    Input::add_key( 32, GLFW_KEY_O, "KO");
    Input::add_key( 33, GLFW_KEY_P, "KP");
    Input::add_key( 34, GLFW_KEY_Q, "KQ");
    Input::add_key( 35, GLFW_KEY_R, "KR");
    Input::add_key( 36, GLFW_KEY_S, "KS");
    Input::add_key( 37, GLFW_KEY_T, "KT");
    Input::add_key( 38, GLFW_KEY_U, "KU");
    Input::add_key( 39, GLFW_KEY_V, "KV");
    Input::add_key( 40, GLFW_KEY_W, "KW");
    Input::add_key( 41, GLFW_KEY_X, "KX");
    Input::add_key( 42, GLFW_KEY_Y, "KY");
    Input::add_key( 43, GLFW_KEY_Z, "KZ");
    Input::add_key( 44, GLFW_KEY_LEFT_BRACKET , "KLEFT_BRACKET");
    Input::add_key( 45, GLFW_KEY_BACKSLASH , "KBACKSLASH");
    Input::add_key( 46, GLFW_KEY_RIGHT_BRACKET , "KRIGHT_BRACKET");
    Input::add_key( 47, GLFW_KEY_GRAVE_ACCENT , "KGRAVE_ACCENT");
    Input::add_key( 48, GLFW_KEY_WORLD_1 , "KWORLD_1");
    Input::add_key( 49, GLFW_KEY_WORLD_2 , "KWORLD_2");
    Input::add_key( 50, GLFW_KEY_ESCAPE, "KESCAPE");
    Input::add_key( 51, GLFW_KEY_ENTER, "KENTER");
    Input::add_key( 52, GLFW_KEY_TAB, "KTAB");
    Input::add_key( 53, GLFW_KEY_BACKSPACE, "KBACKSPACE");
    Input::add_key( 54, GLFW_KEY_INSERT, "KINSERT");
    Input::add_key( 55, GLFW_KEY_DELETE, "KDELETE");
    Input::add_key( 56, GLFW_KEY_RIGHT, "KRIGHT");
    Input::add_key( 57, GLFW_KEY_LEFT, "KLEFT");
    Input::add_key( 58, GLFW_KEY_DOWN, "KDOWN");
    Input::add_key( 59, GLFW_KEY_UP, "KUP");
    Input::add_key( 60, GLFW_KEY_PAGE_UP, "KPAGE_UP");
    Input::add_key( 61, GLFW_KEY_PAGE_DOWN, "KPAGE_DOWN");
    Input::add_key( 62, GLFW_KEY_HOME, "KHOME");
    Input::add_key( 63, GLFW_KEY_END, "KEND");
    Input::add_key( 64, GLFW_KEY_CAPS_LOCK, "KCAPS_LOCK");
    Input::add_key( 65, GLFW_KEY_SCROLL_LOCK, "KSCROLL_LOCK");
    Input::add_key( 66, GLFW_KEY_NUM_LOCK, "KNUM_LOCK");
    Input::add_key( 67, GLFW_KEY_PRINT_SCREEN, "KPRINT_SCREEN");
    Input::add_key( 68, GLFW_KEY_PAUSE, "KPAUSE");
    Input::add_key( 69, GLFW_KEY_F1, "KF1");
    Input::add_key( 70, GLFW_KEY_F2, "KF2");
    Input::add_key( 71, GLFW_KEY_F3, "KF3");
    Input::add_key( 72, GLFW_KEY_F4, "KF4");
    Input::add_key( 73, GLFW_KEY_F5, "KF5");
    Input::add_key( 74, GLFW_KEY_F6, "KF6");
    Input::add_key( 75, GLFW_KEY_F7, "KF7");
    Input::add_key( 76, GLFW_KEY_F8, "KF8");
    Input::add_key( 77, GLFW_KEY_F9, "KF9");
    Input::add_key( 78, GLFW_KEY_F10, "KF10");
    Input::add_key( 79, GLFW_KEY_F11, "KF11");
    Input::add_key( 80, GLFW_KEY_F12, "KF12");
    Input::add_key( 81, GLFW_KEY_F13, "KF13");
    Input::add_key( 82, GLFW_KEY_F14, "KF14");
    Input::add_key( 83, GLFW_KEY_F15, "KF15");
    Input::add_key( 84, GLFW_KEY_F16, "KF16");
    Input::add_key( 85, GLFW_KEY_F17, "KF17");
    Input::add_key( 86, GLFW_KEY_F18, "KF18");
    Input::add_key( 87, GLFW_KEY_F19, "KF19");
    Input::add_key( 88, GLFW_KEY_F20, "KF20");
    Input::add_key( 89, GLFW_KEY_F21, "KF21");
    Input::add_key( 90, GLFW_KEY_F22, "KF22");
    Input::add_key( 91, GLFW_KEY_F23, "KF23");
    Input::add_key( 92, GLFW_KEY_F24, "KF24");
    Input::add_key( 93, GLFW_KEY_F25, "KF25");
    Input::add_key( 94, GLFW_KEY_KP_0, "KKP_0");
    Input::add_key( 95, GLFW_KEY_KP_1, "KKP_1");
    Input::add_key( 96, GLFW_KEY_KP_2, "KKP_2");
    Input::add_key( 97, GLFW_KEY_KP_3, "KKP_3");
    Input::add_key( 98, GLFW_KEY_KP_4, "KKP_4");
    Input::add_key( 99, GLFW_KEY_KP_5, "KKP_5");
    Input::add_key(100, GLFW_KEY_KP_6, "KKP_6");
    Input::add_key(101, GLFW_KEY_KP_7, "KKP_7");
    Input::add_key(102, GLFW_KEY_KP_8, "KKP_8");
    Input::add_key(103, GLFW_KEY_KP_9, "KKP_9");
    Input::add_key(104, GLFW_KEY_KP_DECIMAL, "KKP_DECIMAL");
    Input::add_key(105, GLFW_KEY_KP_DIVIDE, "KKP_DIVIDE");
    Input::add_key(106, GLFW_KEY_KP_MULTIPLY, "KKP_MULTIPLY");
    Input::add_key(107, GLFW_KEY_KP_SUBTRACT, "KKP_SUBTRACT");
    Input::add_key(108, GLFW_KEY_KP_ADD, "KKP_ADD");
    Input::add_key(109, GLFW_KEY_KP_ENTER, "KKP_ENTER");
    Input::add_key(110, GLFW_KEY_KP_EQUAL, "KKP_EQUAL");
    Input::add_key(111, GLFW_KEY_LEFT_SHIFT, "KLEFT_SHIFT");
    Input::add_key(112, GLFW_KEY_LEFT_CONTROL, "KLEFT_CONTROL");
    Input::add_key(113, GLFW_KEY_LEFT_ALT, "KLEFT_ALT");
    Input::add_key(114, GLFW_KEY_LEFT_SUPER, "KLEFT_SUPER");
    Input::add_key(115, GLFW_KEY_RIGHT_SHIFT, "KRIGHT_SHIFT");
    Input::add_key(116, GLFW_KEY_RIGHT_CONTROL, "KRIGHT_CONTROL");
    Input::add_key(117, GLFW_KEY_RIGHT_ALT, "KRIGHT_ALT");
    Input::add_key(118, GLFW_KEY_RIGHT_SUPER, "KRIGHT_SUPER");
    Input::add_key(119, GLFW_KEY_MENU, "KMENU");
  }

  KeyboardKey* Input::get_key(int key_id) {
    return &keyboard_keys[key_id];
  }

  void Input::update(GLFWwindow* window) {
    for(int i = 0; i < 120; i++) {
      keyboard_keys[i].state = glfwGetKey(window, keyboard_keys[i].key);
    }
  }

  bool Input::key_pressed(int key_index) {
    return keyboard_keys[key_index].state == GLFW_PRESS;
  }

  bool Input::key_released(int key_index) {
    return keyboard_keys[key_index].state == GLFW_RELEASE;
  }

}