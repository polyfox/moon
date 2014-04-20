#include "mrb.hxx"
#include "input.hxx"

namespace Moon {

  static mrb_value
  moon_mrb_mouse_x(mrb_state *mrb, mrb_value self) {
    return mrb_fixnum_value(Input::Mouse::x());
  }

  static mrb_value
  moon_mrb_mouse_y(mrb_state *mrb, mrb_value self) {
    return mrb_fixnum_value(Input::Mouse::y());
  }

  static mrb_value
  moon_mrb_mouse_pos(mrb_state *mrb, mrb_value self) {
    auto pos = Input::Mouse::pos();

    mrb_value ary = mrb_ary_new(mrb);
    mrb_ary_push(mrb, ary, mrb_fixnum_value(pos[0]));
    mrb_ary_push(mrb, ary, mrb_fixnum_value(pos[1]));
    return ary;
  }

  struct RClass*
  moon_mrb_input_init(mrb_state *mrb) {
    struct RClass *input_module;
    struct RClass *mouse_module;

    input_module = mrb_define_module_under(mrb, moon_module, "Input");
    mouse_module = mrb_define_module_under(mrb, input_module, "Mouse");

    // mouse functions
    mrb_define_class_method(mrb, mouse_module, "x",   moon_mrb_mouse_x,   MRB_ARGS_NONE());
    mrb_define_class_method(mrb, mouse_module, "y",   moon_mrb_mouse_y,   MRB_ARGS_NONE());
    mrb_define_class_method(mrb, mouse_module, "pos", moon_mrb_mouse_pos, MRB_ARGS_NONE());

    // input constants
    mrb_define_const(mrb, input_module, "SPACE",   mrb_fixnum_value(GLFW_KEY_SPACE));
    mrb_define_const(mrb, input_module, "APOSTROPHE",   mrb_fixnum_value(GLFW_KEY_APOSTROPHE));
    mrb_define_const(mrb, input_module, "COMMA",   mrb_fixnum_value(GLFW_KEY_COMMA));
    mrb_define_const(mrb, input_module, "MINUS",   mrb_fixnum_value(GLFW_KEY_MINUS));
    mrb_define_const(mrb, input_module, "PERIOD",   mrb_fixnum_value(GLFW_KEY_PERIOD));
    mrb_define_const(mrb, input_module, "SLASH",   mrb_fixnum_value(GLFW_KEY_SLASH));
    mrb_define_const(mrb, input_module, "N0",   mrb_fixnum_value(GLFW_KEY_0)); /* cannot define ruby numeric constant, so we prefix N */
    mrb_define_const(mrb, input_module, "N1",   mrb_fixnum_value(GLFW_KEY_1));
    mrb_define_const(mrb, input_module, "N2",   mrb_fixnum_value(GLFW_KEY_2));
    mrb_define_const(mrb, input_module, "N3",   mrb_fixnum_value(GLFW_KEY_3));
    mrb_define_const(mrb, input_module, "N4",   mrb_fixnum_value(GLFW_KEY_4));
    mrb_define_const(mrb, input_module, "N5",   mrb_fixnum_value(GLFW_KEY_5));
    mrb_define_const(mrb, input_module, "N6",   mrb_fixnum_value(GLFW_KEY_6));
    mrb_define_const(mrb, input_module, "N7",   mrb_fixnum_value(GLFW_KEY_7));
    mrb_define_const(mrb, input_module, "N8",   mrb_fixnum_value(GLFW_KEY_8));
    mrb_define_const(mrb, input_module, "N9",   mrb_fixnum_value(GLFW_KEY_9));
    mrb_define_const(mrb, input_module, "SEMICOLON",   mrb_fixnum_value(GLFW_KEY_SEMICOLON));
    mrb_define_const(mrb, input_module, "EQUAL",   mrb_fixnum_value(GLFW_KEY_EQUAL));
    mrb_define_const(mrb, input_module, "A",   mrb_fixnum_value(GLFW_KEY_A));
    mrb_define_const(mrb, input_module, "B",   mrb_fixnum_value(GLFW_KEY_B));
    mrb_define_const(mrb, input_module, "C",   mrb_fixnum_value(GLFW_KEY_C));
    mrb_define_const(mrb, input_module, "D",   mrb_fixnum_value(GLFW_KEY_D));
    mrb_define_const(mrb, input_module, "E",   mrb_fixnum_value(GLFW_KEY_E));
    mrb_define_const(mrb, input_module, "F",   mrb_fixnum_value(GLFW_KEY_F));
    mrb_define_const(mrb, input_module, "G",   mrb_fixnum_value(GLFW_KEY_G));
    mrb_define_const(mrb, input_module, "H",   mrb_fixnum_value(GLFW_KEY_H));
    mrb_define_const(mrb, input_module, "I",   mrb_fixnum_value(GLFW_KEY_I));
    mrb_define_const(mrb, input_module, "J",   mrb_fixnum_value(GLFW_KEY_J));
    mrb_define_const(mrb, input_module, "K",   mrb_fixnum_value(GLFW_KEY_K));
    mrb_define_const(mrb, input_module, "L",   mrb_fixnum_value(GLFW_KEY_L));
    mrb_define_const(mrb, input_module, "M",   mrb_fixnum_value(GLFW_KEY_M));
    mrb_define_const(mrb, input_module, "N",   mrb_fixnum_value(GLFW_KEY_N));
    mrb_define_const(mrb, input_module, "O",   mrb_fixnum_value(GLFW_KEY_O));
    mrb_define_const(mrb, input_module, "P",   mrb_fixnum_value(GLFW_KEY_P));
    mrb_define_const(mrb, input_module, "Q",   mrb_fixnum_value(GLFW_KEY_Q));
    mrb_define_const(mrb, input_module, "R",   mrb_fixnum_value(GLFW_KEY_R));
    mrb_define_const(mrb, input_module, "S",   mrb_fixnum_value(GLFW_KEY_S));
    mrb_define_const(mrb, input_module, "T",   mrb_fixnum_value(GLFW_KEY_T));
    mrb_define_const(mrb, input_module, "U",   mrb_fixnum_value(GLFW_KEY_U));
    mrb_define_const(mrb, input_module, "V",   mrb_fixnum_value(GLFW_KEY_V));
    mrb_define_const(mrb, input_module, "W",   mrb_fixnum_value(GLFW_KEY_W));
    mrb_define_const(mrb, input_module, "X",   mrb_fixnum_value(GLFW_KEY_X));
    mrb_define_const(mrb, input_module, "Y",   mrb_fixnum_value(GLFW_KEY_Y));
    mrb_define_const(mrb, input_module, "Z",   mrb_fixnum_value(GLFW_KEY_Z));
    mrb_define_const(mrb, input_module, "LEFT_BRACKET",   mrb_fixnum_value(GLFW_KEY_LEFT_BRACKET));
    mrb_define_const(mrb, input_module, "BACKSLASH",   mrb_fixnum_value(GLFW_KEY_BACKSLASH));
    mrb_define_const(mrb, input_module, "RIGHT_BRACKET",   mrb_fixnum_value(GLFW_KEY_RIGHT_BRACKET));
    mrb_define_const(mrb, input_module, "GRAVE_ACCENT",   mrb_fixnum_value(GLFW_KEY_GRAVE_ACCENT));
    mrb_define_const(mrb, input_module, "WORLD_1",   mrb_fixnum_value(GLFW_KEY_WORLD_1));
    mrb_define_const(mrb, input_module, "WORLD_2",   mrb_fixnum_value(GLFW_KEY_WORLD_2));
    mrb_define_const(mrb, input_module, "ESCAPE",   mrb_fixnum_value(GLFW_KEY_ESCAPE));
    mrb_define_const(mrb, input_module, "ENTER",   mrb_fixnum_value(GLFW_KEY_ENTER));
    mrb_define_const(mrb, input_module, "TAB",   mrb_fixnum_value(GLFW_KEY_TAB));
    mrb_define_const(mrb, input_module, "BACKSPACE",   mrb_fixnum_value(GLFW_KEY_BACKSPACE));
    mrb_define_const(mrb, input_module, "INSERT",   mrb_fixnum_value(GLFW_KEY_INSERT));
    mrb_define_const(mrb, input_module, "DELETE",   mrb_fixnum_value(GLFW_KEY_DELETE));
    mrb_define_const(mrb, input_module, "RIGHT",   mrb_fixnum_value(GLFW_KEY_RIGHT));
    mrb_define_const(mrb, input_module, "LEFT",   mrb_fixnum_value(GLFW_KEY_LEFT));
    mrb_define_const(mrb, input_module, "DOWN",   mrb_fixnum_value(GLFW_KEY_DOWN));
    mrb_define_const(mrb, input_module, "UP",   mrb_fixnum_value(GLFW_KEY_UP));
    mrb_define_const(mrb, input_module, "PAGE_UP",   mrb_fixnum_value(GLFW_KEY_PAGE_UP));
    mrb_define_const(mrb, input_module, "PAGE_DOWN",   mrb_fixnum_value(GLFW_KEY_PAGE_DOWN));
    mrb_define_const(mrb, input_module, "HOME",   mrb_fixnum_value(GLFW_KEY_HOME));
    mrb_define_const(mrb, input_module, "END",   mrb_fixnum_value(GLFW_KEY_END));
    mrb_define_const(mrb, input_module, "CAPS_LOCK",   mrb_fixnum_value(GLFW_KEY_CAPS_LOCK));
    mrb_define_const(mrb, input_module, "SCROLL_LOCK",   mrb_fixnum_value(GLFW_KEY_SCROLL_LOCK));
    mrb_define_const(mrb, input_module, "NUM_LOCK",   mrb_fixnum_value(GLFW_KEY_NUM_LOCK));
    mrb_define_const(mrb, input_module, "PRINT_SCREEN",   mrb_fixnum_value(GLFW_KEY_PRINT_SCREEN));
    mrb_define_const(mrb, input_module, "PAUSE",   mrb_fixnum_value(GLFW_KEY_PAUSE));
    mrb_define_const(mrb, input_module, "F1",   mrb_fixnum_value(GLFW_KEY_F1));
    mrb_define_const(mrb, input_module, "F2",   mrb_fixnum_value(GLFW_KEY_F2));
    mrb_define_const(mrb, input_module, "F3",   mrb_fixnum_value(GLFW_KEY_F3));
    mrb_define_const(mrb, input_module, "F4",   mrb_fixnum_value(GLFW_KEY_F4));
    mrb_define_const(mrb, input_module, "F5",   mrb_fixnum_value(GLFW_KEY_F5));
    mrb_define_const(mrb, input_module, "F6",   mrb_fixnum_value(GLFW_KEY_F6));
    mrb_define_const(mrb, input_module, "F7",   mrb_fixnum_value(GLFW_KEY_F7));
    mrb_define_const(mrb, input_module, "F8",   mrb_fixnum_value(GLFW_KEY_F8));
    mrb_define_const(mrb, input_module, "F9",   mrb_fixnum_value(GLFW_KEY_F9));
    mrb_define_const(mrb, input_module, "F10",   mrb_fixnum_value(GLFW_KEY_F10));
    mrb_define_const(mrb, input_module, "F11",   mrb_fixnum_value(GLFW_KEY_F11));
    mrb_define_const(mrb, input_module, "F12",   mrb_fixnum_value(GLFW_KEY_F12));
    mrb_define_const(mrb, input_module, "F13",   mrb_fixnum_value(GLFW_KEY_F13));
    mrb_define_const(mrb, input_module, "F14",   mrb_fixnum_value(GLFW_KEY_F14));
    mrb_define_const(mrb, input_module, "F15",   mrb_fixnum_value(GLFW_KEY_F15));
    mrb_define_const(mrb, input_module, "F16",   mrb_fixnum_value(GLFW_KEY_F16));
    mrb_define_const(mrb, input_module, "F17",   mrb_fixnum_value(GLFW_KEY_F17));
    mrb_define_const(mrb, input_module, "F18",   mrb_fixnum_value(GLFW_KEY_F18));
    mrb_define_const(mrb, input_module, "F19",   mrb_fixnum_value(GLFW_KEY_F19));
    mrb_define_const(mrb, input_module, "F20",   mrb_fixnum_value(GLFW_KEY_F20));
    mrb_define_const(mrb, input_module, "F21",   mrb_fixnum_value(GLFW_KEY_F21));
    mrb_define_const(mrb, input_module, "F22",   mrb_fixnum_value(GLFW_KEY_F22));
    mrb_define_const(mrb, input_module, "F23",   mrb_fixnum_value(GLFW_KEY_F23));
    mrb_define_const(mrb, input_module, "F24",   mrb_fixnum_value(GLFW_KEY_F24));
    mrb_define_const(mrb, input_module, "F25",   mrb_fixnum_value(GLFW_KEY_F25));
    mrb_define_const(mrb, input_module, "KP_0",   mrb_fixnum_value(GLFW_KEY_KP_0));
    mrb_define_const(mrb, input_module, "KP_1",   mrb_fixnum_value(GLFW_KEY_KP_1));
    mrb_define_const(mrb, input_module, "KP_2",   mrb_fixnum_value(GLFW_KEY_KP_2));
    mrb_define_const(mrb, input_module, "KP_3",   mrb_fixnum_value(GLFW_KEY_KP_3));
    mrb_define_const(mrb, input_module, "KP_4",   mrb_fixnum_value(GLFW_KEY_KP_4));
    mrb_define_const(mrb, input_module, "KP_5",   mrb_fixnum_value(GLFW_KEY_KP_5));
    mrb_define_const(mrb, input_module, "KP_6",   mrb_fixnum_value(GLFW_KEY_KP_6));
    mrb_define_const(mrb, input_module, "KP_7",   mrb_fixnum_value(GLFW_KEY_KP_7));
    mrb_define_const(mrb, input_module, "KP_8",   mrb_fixnum_value(GLFW_KEY_KP_8));
    mrb_define_const(mrb, input_module, "KP_9",   mrb_fixnum_value(GLFW_KEY_KP_9));
    mrb_define_const(mrb, input_module, "KP_DECIMAL",   mrb_fixnum_value(GLFW_KEY_KP_DECIMAL));
    mrb_define_const(mrb, input_module, "KP_DIVIDE",   mrb_fixnum_value(GLFW_KEY_KP_DIVIDE));
    mrb_define_const(mrb, input_module, "KP_MULTIPLY",   mrb_fixnum_value(GLFW_KEY_KP_MULTIPLY));
    mrb_define_const(mrb, input_module, "KP_SUBTRACT",   mrb_fixnum_value(GLFW_KEY_KP_SUBTRACT));
    mrb_define_const(mrb, input_module, "KP_ADD",   mrb_fixnum_value(GLFW_KEY_KP_ADD));
    mrb_define_const(mrb, input_module, "KP_ENTER",   mrb_fixnum_value(GLFW_KEY_KP_ENTER));
    mrb_define_const(mrb, input_module, "KP_EQUAL",   mrb_fixnum_value(GLFW_KEY_KP_EQUAL));
    mrb_define_const(mrb, input_module, "LEFT_SHIFT",   mrb_fixnum_value(GLFW_KEY_LEFT_SHIFT));
    mrb_define_const(mrb, input_module, "LEFT_CONTROL",   mrb_fixnum_value(GLFW_KEY_LEFT_CONTROL));
    mrb_define_const(mrb, input_module, "LEFT_ALT",   mrb_fixnum_value(GLFW_KEY_LEFT_ALT));
    mrb_define_const(mrb, input_module, "LEFT_SUPER",   mrb_fixnum_value(GLFW_KEY_LEFT_SUPER));
    mrb_define_const(mrb, input_module, "RIGHT_SHIFT",   mrb_fixnum_value(GLFW_KEY_RIGHT_SHIFT));
    mrb_define_const(mrb, input_module, "RIGHT_CONTROL",   mrb_fixnum_value(GLFW_KEY_RIGHT_CONTROL));
    mrb_define_const(mrb, input_module, "RIGHT_ALT",   mrb_fixnum_value(GLFW_KEY_RIGHT_ALT));
    mrb_define_const(mrb, input_module, "RIGHT_SUPER",   mrb_fixnum_value(GLFW_KEY_RIGHT_SUPER));
    mrb_define_const(mrb, input_module, "MENU",   mrb_fixnum_value(GLFW_KEY_MENU));

    // key modifiers
    mrb_define_const(mrb, input_module, "MOD_SHIFT",   mrb_fixnum_value(GLFW_MOD_SHIFT));
    mrb_define_const(mrb, input_module, "MOD_CONTROL", mrb_fixnum_value(GLFW_MOD_CONTROL));
    mrb_define_const(mrb, input_module, "MOD_ALT",     mrb_fixnum_value(GLFW_MOD_ALT));
    mrb_define_const(mrb, input_module, "MOD_SUPER",   mrb_fixnum_value(GLFW_MOD_SUPER));

    // mouse constants
    mrb_define_const(mrb, input_module, "MOUSE_BUTTON_1", mrb_fixnum_value(GLFW_MOUSE_BUTTON_1));
    mrb_define_const(mrb, input_module, "MOUSE_BUTTON_2", mrb_fixnum_value(GLFW_MOUSE_BUTTON_2));
    mrb_define_const(mrb, input_module, "MOUSE_BUTTON_3", mrb_fixnum_value(GLFW_MOUSE_BUTTON_3));
    mrb_define_const(mrb, input_module, "MOUSE_BUTTON_4", mrb_fixnum_value(GLFW_MOUSE_BUTTON_4));
    mrb_define_const(mrb, input_module, "MOUSE_BUTTON_5", mrb_fixnum_value(GLFW_MOUSE_BUTTON_5));
    mrb_define_const(mrb, input_module, "MOUSE_BUTTON_6", mrb_fixnum_value(GLFW_MOUSE_BUTTON_6));
    mrb_define_const(mrb, input_module, "MOUSE_BUTTON_7", mrb_fixnum_value(GLFW_MOUSE_BUTTON_7));
    mrb_define_const(mrb, input_module, "MOUSE_BUTTON_8", mrb_fixnum_value(GLFW_MOUSE_BUTTON_8));

    // alias constants
    mrb_define_const(mrb, input_module, "MOUSE_LEFT",   mrb_fixnum_value(GLFW_MOUSE_BUTTON_LEFT));
    mrb_define_const(mrb, input_module, "MOUSE_RIGHT",  mrb_fixnum_value(GLFW_MOUSE_BUTTON_RIGHT));
    mrb_define_const(mrb, input_module, "MOUSE_MIDDLE", mrb_fixnum_value(GLFW_MOUSE_BUTTON_MIDDLE));

    return input_module;
  }

}
/*
  mruby.api reference
    void mrb_define_const(mrb_state*, struct RClass*, const char *name, mrb_value);
*/