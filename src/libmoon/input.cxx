#include "moon/input.hxx"
#include "moon/mrb/input.hxx"

namespace Moon {
  GLFWwindow* Input::window = NULL;
  mrb_state* Input::mrb = NULL;

  int__mrb_value_map_t Input::glfw_state_map;
  int__mrb_value_map_t Input::glfw_key_map;

  void Input::initialize(GLFWwindow* var_window, mrb_state* var_mrb) {
    window = var_window;
    mrb = var_mrb;
    glfwSetKeyCallback(window, Input::update_key);
    glfwSetMouseButtonCallback(window, Input::update_button);
    glfwSetCharCallback(window, Input::on_type);
    glfwSetCursorPosCallback(window, Input::update_cursor_pos);

    initialize_maps();
  }

  void Input::initialize_maps() {
    /* GLFW Key State */
    glfw_state_map[GLFW_PRESS]   = mrb_symbol_value(mrb_intern_cstr(mrb, "press"));
    glfw_state_map[GLFW_RELEASE] = mrb_symbol_value(mrb_intern_cstr(mrb, "release"));
    glfw_state_map[GLFW_REPEAT]  = mrb_symbol_value(mrb_intern_cstr(mrb, "repeat"));

    /* GLFW Key Map */
    glfw_key_map[GLFW_KEY_SPACE] = mrb_symbol_value(mrb_intern_cstr(mrb, "space"));
    glfw_key_map[GLFW_KEY_APOSTROPHE] = mrb_symbol_value(mrb_intern_cstr(mrb, "apostrophe"));
    glfw_key_map[GLFW_KEY_COMMA] = mrb_symbol_value(mrb_intern_cstr(mrb, "comma"));
    glfw_key_map[GLFW_KEY_MINUS] = mrb_symbol_value(mrb_intern_cstr(mrb, "minus"));
    glfw_key_map[GLFW_KEY_PERIOD] = mrb_symbol_value(mrb_intern_cstr(mrb, "period"));
    glfw_key_map[GLFW_KEY_SLASH] = mrb_symbol_value(mrb_intern_cstr(mrb, "slash"));
    glfw_key_map[GLFW_KEY_0] = mrb_symbol_value(mrb_intern_cstr(mrb, "n0"));
    glfw_key_map[GLFW_KEY_1] = mrb_symbol_value(mrb_intern_cstr(mrb, "n1"));
    glfw_key_map[GLFW_KEY_2] = mrb_symbol_value(mrb_intern_cstr(mrb, "n2"));
    glfw_key_map[GLFW_KEY_3] = mrb_symbol_value(mrb_intern_cstr(mrb, "n3"));
    glfw_key_map[GLFW_KEY_4] = mrb_symbol_value(mrb_intern_cstr(mrb, "n4"));
    glfw_key_map[GLFW_KEY_5] = mrb_symbol_value(mrb_intern_cstr(mrb, "n5"));
    glfw_key_map[GLFW_KEY_6] = mrb_symbol_value(mrb_intern_cstr(mrb, "n6"));
    glfw_key_map[GLFW_KEY_7] = mrb_symbol_value(mrb_intern_cstr(mrb, "n7"));
    glfw_key_map[GLFW_KEY_8] = mrb_symbol_value(mrb_intern_cstr(mrb, "n8"));
    glfw_key_map[GLFW_KEY_9] = mrb_symbol_value(mrb_intern_cstr(mrb, "n9"));
    glfw_key_map[GLFW_KEY_SEMICOLON] = mrb_symbol_value(mrb_intern_cstr(mrb, "semicolon"));
    glfw_key_map[GLFW_KEY_EQUAL] = mrb_symbol_value(mrb_intern_cstr(mrb, "equal"));
    glfw_key_map[GLFW_KEY_A] = mrb_symbol_value(mrb_intern_cstr(mrb, "a"));
    glfw_key_map[GLFW_KEY_B] = mrb_symbol_value(mrb_intern_cstr(mrb, "b"));
    glfw_key_map[GLFW_KEY_C] = mrb_symbol_value(mrb_intern_cstr(mrb, "c"));
    glfw_key_map[GLFW_KEY_D] = mrb_symbol_value(mrb_intern_cstr(mrb, "d"));
    glfw_key_map[GLFW_KEY_E] = mrb_symbol_value(mrb_intern_cstr(mrb, "e"));
    glfw_key_map[GLFW_KEY_F] = mrb_symbol_value(mrb_intern_cstr(mrb, "f"));
    glfw_key_map[GLFW_KEY_G] = mrb_symbol_value(mrb_intern_cstr(mrb, "g"));
    glfw_key_map[GLFW_KEY_H] = mrb_symbol_value(mrb_intern_cstr(mrb, "h"));
    glfw_key_map[GLFW_KEY_I] = mrb_symbol_value(mrb_intern_cstr(mrb, "i"));
    glfw_key_map[GLFW_KEY_J] = mrb_symbol_value(mrb_intern_cstr(mrb, "j"));
    glfw_key_map[GLFW_KEY_K] = mrb_symbol_value(mrb_intern_cstr(mrb, "k"));
    glfw_key_map[GLFW_KEY_L] = mrb_symbol_value(mrb_intern_cstr(mrb, "l"));
    glfw_key_map[GLFW_KEY_M] = mrb_symbol_value(mrb_intern_cstr(mrb, "m"));
    glfw_key_map[GLFW_KEY_N] = mrb_symbol_value(mrb_intern_cstr(mrb, "n"));
    glfw_key_map[GLFW_KEY_O] = mrb_symbol_value(mrb_intern_cstr(mrb, "o"));
    glfw_key_map[GLFW_KEY_P] = mrb_symbol_value(mrb_intern_cstr(mrb, "p"));
    glfw_key_map[GLFW_KEY_Q] = mrb_symbol_value(mrb_intern_cstr(mrb, "q"));
    glfw_key_map[GLFW_KEY_R] = mrb_symbol_value(mrb_intern_cstr(mrb, "r"));
    glfw_key_map[GLFW_KEY_S] = mrb_symbol_value(mrb_intern_cstr(mrb, "s"));
    glfw_key_map[GLFW_KEY_T] = mrb_symbol_value(mrb_intern_cstr(mrb, "t"));
    glfw_key_map[GLFW_KEY_U] = mrb_symbol_value(mrb_intern_cstr(mrb, "u"));
    glfw_key_map[GLFW_KEY_V] = mrb_symbol_value(mrb_intern_cstr(mrb, "v"));
    glfw_key_map[GLFW_KEY_W] = mrb_symbol_value(mrb_intern_cstr(mrb, "w"));
    glfw_key_map[GLFW_KEY_X] = mrb_symbol_value(mrb_intern_cstr(mrb, "x"));
    glfw_key_map[GLFW_KEY_Y] = mrb_symbol_value(mrb_intern_cstr(mrb, "y"));
    glfw_key_map[GLFW_KEY_Z] = mrb_symbol_value(mrb_intern_cstr(mrb, "z"));
    glfw_key_map[GLFW_KEY_LEFT_BRACKET] = mrb_symbol_value(mrb_intern_cstr(mrb, "left_bracket"));
    glfw_key_map[GLFW_KEY_BACKSLASH] = mrb_symbol_value(mrb_intern_cstr(mrb, "backslash"));
    glfw_key_map[GLFW_KEY_RIGHT_BRACKET] = mrb_symbol_value(mrb_intern_cstr(mrb, "right_bracket"));
    glfw_key_map[GLFW_KEY_GRAVE_ACCENT] = mrb_symbol_value(mrb_intern_cstr(mrb, "grave_accent"));
    glfw_key_map[GLFW_KEY_WORLD_1] = mrb_symbol_value(mrb_intern_cstr(mrb, "world_1"));
    glfw_key_map[GLFW_KEY_WORLD_2] = mrb_symbol_value(mrb_intern_cstr(mrb, "world_2"));
    glfw_key_map[GLFW_KEY_ESCAPE] = mrb_symbol_value(mrb_intern_cstr(mrb, "escape"));
    glfw_key_map[GLFW_KEY_ENTER] = mrb_symbol_value(mrb_intern_cstr(mrb, "enter"));
    glfw_key_map[GLFW_KEY_TAB] = mrb_symbol_value(mrb_intern_cstr(mrb, "tab"));
    glfw_key_map[GLFW_KEY_BACKSPACE] = mrb_symbol_value(mrb_intern_cstr(mrb, "backspace"));
    glfw_key_map[GLFW_KEY_INSERT] = mrb_symbol_value(mrb_intern_cstr(mrb, "insert"));
    glfw_key_map[GLFW_KEY_DELETE] = mrb_symbol_value(mrb_intern_cstr(mrb, "delete"));
    glfw_key_map[GLFW_KEY_RIGHT] = mrb_symbol_value(mrb_intern_cstr(mrb, "right"));
    glfw_key_map[GLFW_KEY_LEFT] = mrb_symbol_value(mrb_intern_cstr(mrb, "left"));
    glfw_key_map[GLFW_KEY_DOWN] = mrb_symbol_value(mrb_intern_cstr(mrb, "down"));
    glfw_key_map[GLFW_KEY_UP] = mrb_symbol_value(mrb_intern_cstr(mrb, "up"));
    glfw_key_map[GLFW_KEY_PAGE_UP] = mrb_symbol_value(mrb_intern_cstr(mrb, "page_up"));
    glfw_key_map[GLFW_KEY_PAGE_DOWN] = mrb_symbol_value(mrb_intern_cstr(mrb, "page_down"));
    glfw_key_map[GLFW_KEY_HOME] = mrb_symbol_value(mrb_intern_cstr(mrb, "home"));
    glfw_key_map[GLFW_KEY_END] = mrb_symbol_value(mrb_intern_cstr(mrb, "end"));
    glfw_key_map[GLFW_KEY_CAPS_LOCK] = mrb_symbol_value(mrb_intern_cstr(mrb, "caps_lock"));
    glfw_key_map[GLFW_KEY_SCROLL_LOCK] = mrb_symbol_value(mrb_intern_cstr(mrb, "scroll_lock"));
    glfw_key_map[GLFW_KEY_NUM_LOCK] = mrb_symbol_value(mrb_intern_cstr(mrb, "num_lock"));
    glfw_key_map[GLFW_KEY_PRINT_SCREEN] = mrb_symbol_value(mrb_intern_cstr(mrb, "print_screen"));
    glfw_key_map[GLFW_KEY_PAUSE] = mrb_symbol_value(mrb_intern_cstr(mrb, "pause"));
    glfw_key_map[GLFW_KEY_F1] = mrb_symbol_value(mrb_intern_cstr(mrb, "f1"));
    glfw_key_map[GLFW_KEY_F2] = mrb_symbol_value(mrb_intern_cstr(mrb, "f2"));
    glfw_key_map[GLFW_KEY_F3] = mrb_symbol_value(mrb_intern_cstr(mrb, "f3"));
    glfw_key_map[GLFW_KEY_F4] = mrb_symbol_value(mrb_intern_cstr(mrb, "f4"));
    glfw_key_map[GLFW_KEY_F5] = mrb_symbol_value(mrb_intern_cstr(mrb, "f5"));
    glfw_key_map[GLFW_KEY_F6] = mrb_symbol_value(mrb_intern_cstr(mrb, "f6"));
    glfw_key_map[GLFW_KEY_F7] = mrb_symbol_value(mrb_intern_cstr(mrb, "f7"));
    glfw_key_map[GLFW_KEY_F8] = mrb_symbol_value(mrb_intern_cstr(mrb, "f8"));
    glfw_key_map[GLFW_KEY_F9] = mrb_symbol_value(mrb_intern_cstr(mrb, "f9"));
    glfw_key_map[GLFW_KEY_F10] = mrb_symbol_value(mrb_intern_cstr(mrb, "f10"));
    glfw_key_map[GLFW_KEY_F11] = mrb_symbol_value(mrb_intern_cstr(mrb, "f11"));
    glfw_key_map[GLFW_KEY_F12] = mrb_symbol_value(mrb_intern_cstr(mrb, "f12"));
    glfw_key_map[GLFW_KEY_F13] = mrb_symbol_value(mrb_intern_cstr(mrb, "f13"));
    glfw_key_map[GLFW_KEY_F14] = mrb_symbol_value(mrb_intern_cstr(mrb, "f14"));
    glfw_key_map[GLFW_KEY_F15] = mrb_symbol_value(mrb_intern_cstr(mrb, "f15"));
    glfw_key_map[GLFW_KEY_F16] = mrb_symbol_value(mrb_intern_cstr(mrb, "f16"));
    glfw_key_map[GLFW_KEY_F17] = mrb_symbol_value(mrb_intern_cstr(mrb, "f17"));
    glfw_key_map[GLFW_KEY_F18] = mrb_symbol_value(mrb_intern_cstr(mrb, "f18"));
    glfw_key_map[GLFW_KEY_F19] = mrb_symbol_value(mrb_intern_cstr(mrb, "f19"));
    glfw_key_map[GLFW_KEY_F20] = mrb_symbol_value(mrb_intern_cstr(mrb, "f20"));
    glfw_key_map[GLFW_KEY_F21] = mrb_symbol_value(mrb_intern_cstr(mrb, "f21"));
    glfw_key_map[GLFW_KEY_F22] = mrb_symbol_value(mrb_intern_cstr(mrb, "f22"));
    glfw_key_map[GLFW_KEY_F23] = mrb_symbol_value(mrb_intern_cstr(mrb, "f23"));
    glfw_key_map[GLFW_KEY_F24] = mrb_symbol_value(mrb_intern_cstr(mrb, "f24"));
    glfw_key_map[GLFW_KEY_F25] = mrb_symbol_value(mrb_intern_cstr(mrb, "f25"));
    glfw_key_map[GLFW_KEY_KP_0] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_0"));
    glfw_key_map[GLFW_KEY_KP_1] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_1"));
    glfw_key_map[GLFW_KEY_KP_2] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_2"));
    glfw_key_map[GLFW_KEY_KP_3] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_3"));
    glfw_key_map[GLFW_KEY_KP_4] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_4"));
    glfw_key_map[GLFW_KEY_KP_5] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_5"));
    glfw_key_map[GLFW_KEY_KP_6] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_6"));
    glfw_key_map[GLFW_KEY_KP_7] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_7"));
    glfw_key_map[GLFW_KEY_KP_8] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_8"));
    glfw_key_map[GLFW_KEY_KP_9] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_9"));
    glfw_key_map[GLFW_KEY_KP_DECIMAL] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_decimal"));
    glfw_key_map[GLFW_KEY_KP_DIVIDE] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_divide"));
    glfw_key_map[GLFW_KEY_KP_MULTIPLY] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_multiply"));
    glfw_key_map[GLFW_KEY_KP_SUBTRACT] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_subtract"));
    glfw_key_map[GLFW_KEY_KP_ADD] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_add"));
    glfw_key_map[GLFW_KEY_KP_ENTER] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_enter"));
    glfw_key_map[GLFW_KEY_KP_EQUAL] = mrb_symbol_value(mrb_intern_cstr(mrb, "kp_equal"));
    glfw_key_map[GLFW_KEY_LEFT_SHIFT] = mrb_symbol_value(mrb_intern_cstr(mrb, "left_shift"));
    glfw_key_map[GLFW_KEY_LEFT_CONTROL] = mrb_symbol_value(mrb_intern_cstr(mrb, "left_control"));
    glfw_key_map[GLFW_KEY_LEFT_ALT] = mrb_symbol_value(mrb_intern_cstr(mrb, "left_alt"));
    glfw_key_map[GLFW_KEY_LEFT_SUPER] = mrb_symbol_value(mrb_intern_cstr(mrb, "left_super"));
    glfw_key_map[GLFW_KEY_RIGHT_SHIFT] = mrb_symbol_value(mrb_intern_cstr(mrb, "right_shift"));
    glfw_key_map[GLFW_KEY_RIGHT_CONTROL] = mrb_symbol_value(mrb_intern_cstr(mrb, "right_control"));
    glfw_key_map[GLFW_KEY_RIGHT_ALT] = mrb_symbol_value(mrb_intern_cstr(mrb, "right_alt"));
    glfw_key_map[GLFW_KEY_RIGHT_SUPER] = mrb_symbol_value(mrb_intern_cstr(mrb, "right_super"));
    glfw_key_map[GLFW_KEY_MENU] = mrb_symbol_value(mrb_intern_cstr(mrb, "menu"));
    /*glfw_key_map[GLFW_MOUSE_BUTTON_1] = mrb_symbol_value(mrb_intern_cstr(mrb, "mouse_button_1"));*/
    /*glfw_key_map[GLFW_MOUSE_BUTTON_2] = mrb_symbol_value(mrb_intern_cstr(mrb, "mouse_button_2"));*/
    /*glfw_key_map[GLFW_MOUSE_BUTTON_3] = mrb_symbol_value(mrb_intern_cstr(mrb, "mouse_button_3"));*/
    glfw_key_map[GLFW_MOUSE_BUTTON_4] = mrb_symbol_value(mrb_intern_cstr(mrb, "mouse_button_4"));
    glfw_key_map[GLFW_MOUSE_BUTTON_5] = mrb_symbol_value(mrb_intern_cstr(mrb, "mouse_button_5"));
    glfw_key_map[GLFW_MOUSE_BUTTON_6] = mrb_symbol_value(mrb_intern_cstr(mrb, "mouse_button_6"));
    glfw_key_map[GLFW_MOUSE_BUTTON_7] = mrb_symbol_value(mrb_intern_cstr(mrb, "mouse_button_7"));
    glfw_key_map[GLFW_MOUSE_BUTTON_8] = mrb_symbol_value(mrb_intern_cstr(mrb, "mouse_button_8"));
    glfw_key_map[GLFW_MOUSE_BUTTON_LEFT] = mrb_symbol_value(mrb_intern_cstr(mrb, "mouse_left"));
    glfw_key_map[GLFW_MOUSE_BUTTON_RIGHT] = mrb_symbol_value(mrb_intern_cstr(mrb, "mouse_right"));
    glfw_key_map[GLFW_MOUSE_BUTTON_MIDDLE] = mrb_symbol_value(mrb_intern_cstr(mrb, "mouse_middle"));
  }

  /* Callbacks */

  mrb_value Input::glfw_state_to_mrb_symbol(int state) {
    int__mrb_value_map_t::iterator iter = glfw_state_map.find(state);

    if (iter == glfw_state_map.end())
      return mrb_nil_value();

    return iter->second;
  }

  mrb_value Input::glfw_key_to_mrb_symbol(int key_id) {
    int__mrb_value_map_t::iterator iter = glfw_key_map.find(key_id);

    if (iter == glfw_key_map.end())
      return mrb_nil_value();

    return iter->second;
  }

  void Input::on_type(GLFWwindow* window, unsigned int utf8_char) {
    /* TODO: convert int directly to a UTF8 mruby string */
    mrb_funcall(mrb, mrb_obj_value(mmrb_Input), "on_type", 1,
                mrb_funcall(mrb, mrb_fixnum_value(utf8_char), "chr", 0));
  }

  void Input::update_key(GLFWwindow* window, int key_id, int scancode, int action, int mods) {
    mrb_funcall(mrb, mrb_obj_value(mmrb_Input), "on_key", 4,
      glfw_key_to_mrb_symbol(key_id),
      mrb_fixnum_value(scancode),
      glfw_state_to_mrb_symbol(action),
      mrb_fixnum_value(mods));
  }

  void Input::update_button(GLFWwindow* window, int button_id, int action, int mods) {
    mrb_funcall(mrb, mrb_obj_value(mmrb_Input), "on_button", 3,
      glfw_key_to_mrb_symbol(button_id),
      glfw_state_to_mrb_symbol(action),
      mrb_fixnum_value(mods));
  }

  void Input::update_cursor_pos(GLFWwindow* window, double x, double y) {
    mrb_funcall(mrb, mrb_obj_value(mmrb_Input), "on_mousemove", 2,
      mrb_float_value(mrb, x),
      mrb_float_value(mrb, y));
  }

  /* Mouse functions */
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

  std::array<int, 2> Input::Mouse::get_position() {
    double x, y;
    std::array<int, 2> result;
    glfwGetCursorPos(window, &x, &y);
    result[0] = (int)floor(x);
    result[1] = (int)floor(y);
    return result;
  }
}
