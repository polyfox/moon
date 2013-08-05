#include "mrb.hxx"
#include "input.hxx"

namespace Moon 
{ 
  /* hidden function */
  static mrb_value moon_mrb_input_state_eq_(mrb_state *mrb, mrb_value self, 
                                            int state) {
    mrb_int key_id;
    mrb_int mod_id;
    bool res;
    int argc = mrb_get_args(mrb, "i|i", &key_id, &mod_id);
    if (argc == 2) {
      res = Input::key_state_is_eq_with_mod(key_id, state, mod_id);
    } else {
      res = Input::key_state_is_eq(key_id, state);
    }
    if (res) {
      return mrb_fixnum_value(Input::key_state_hold(key_id, state));
    } else {
      return mrb_bool_value(false);
    }
  }

  static mrb_value moon_mrb_input_is_pressed(mrb_state *mrb, mrb_value self) {
    return moon_mrb_input_state_eq_(mrb, self, GLFW_PRESS);
  }

  static mrb_value moon_mrb_input_is_released(mrb_state *mrb, mrb_value self) {
    return moon_mrb_input_state_eq_(mrb, self, GLFW_RELEASE);
  }

  static mrb_value moon_mrb_input_is_repeated(mrb_state *mrb, mrb_value self) {
    return moon_mrb_input_state_eq_(mrb, self, GLFW_REPEAT);
  }

  static mrb_value moon_mrb_input_is_modded(mrb_state *mrb, mrb_value self) {
    mrb_int key_id;
    mrb_int mod_id;
    mrb_get_args(mrb, "ii", &key_id, &mod_id);
    return mrb_bool_value(Input::key_mod(key_id, mod_id));
  }

  static mrb_value moon_mrb_input_hold(mrb_state *mrb, mrb_value self) {
    mrb_int key_id;
    mrb_int state;
    mrb_get_args(mrb, "ii", &key_id, &state);
    return mrb_fixnum_value(Input::key_state_hold(key_id, state));
  }

  static mrb_value moon_mrb_mouse_x(mrb_state *mrb, mrb_value self) {
    return mrb_fixnum_value(Input::Mouse::x());
  }

  static mrb_value moon_mrb_mouse_y(mrb_state *mrb, mrb_value self) {
    return mrb_fixnum_value(Input::Mouse::y());
  }

  static mrb_value moon_mrb_mouse_state_eq_(mrb_state *mrb, mrb_value self, 
                                            int state) {
    mrb_int button_id;
    mrb_int mod_id;
    bool res;
    int argc = mrb_get_args(mrb, "i|i", &button_id, &mod_id);
    if (argc == 2) {
      res = Input::Mouse::button_state_is_eq_with_mod(button_id, state, mod_id);
    } else {
      res = Input::Mouse::button_state_is_eq(button_id, state);
    }
    if (res) {
      return mrb_fixnum_value(Input::Mouse::button_state_hold(button_id, state));
    } else {
      return mrb_bool_value(false);
    }
  }

  static mrb_value moon_mrb_mouse_is_pressed(mrb_state *mrb, mrb_value self) {
    return moon_mrb_mouse_state_eq_(mrb, self, GLFW_PRESS);
  }

  static mrb_value moon_mrb_mouse_is_released(mrb_state *mrb, mrb_value self) {
    return moon_mrb_mouse_state_eq_(mrb, self, GLFW_RELEASE);
  }

  static mrb_value moon_mrb_mouse_is_modded(mrb_state *mrb, mrb_value self) {
    mrb_int button_id;
    mrb_int mod_id;
    mrb_get_args(mrb, "ii", &button_id, &mod_id);
    return mrb_bool_value(Input::Mouse::button_mod(button_id, mod_id));
  }

  void moon_mrb_input_init(mrb_state *mrb) {
    struct RClass *input_class;
    struct RClass *key_module;
    struct RClass *button_module;
    struct RClass *mouse_module;
    
    input_class   = mrb_define_class(mrb, "Input", mrb->object_class);
    key_module    = mrb_define_module_under(mrb, input_class, "Keys");
    mouse_module  = mrb_define_module_under(mrb, input_class, "Mouse");
    button_module = mrb_define_module_under(mrb, mouse_module, "Buttons");

    // input functions
    mrb_define_class_method(mrb, input_class, "pressed?",  moon_mrb_input_is_pressed,  MRB_ARGS_ARG(1, 1));
    mrb_define_class_method(mrb, input_class, "released?", moon_mrb_input_is_released, MRB_ARGS_ARG(1, 1));
    mrb_define_class_method(mrb, input_class, "repeated?", moon_mrb_input_is_repeated, MRB_ARGS_ARG(1, 1));
    mrb_define_class_method(mrb, input_class, "modded?",   moon_mrb_input_is_modded,   MRB_ARGS_REQ(2));

    // mouse functions
    mrb_define_class_method(mrb, mouse_module, "x", moon_mrb_mouse_x, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, mouse_module, "y", moon_mrb_mouse_y, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, mouse_module, "pressed?",  moon_mrb_mouse_is_pressed,  MRB_ARGS_ARG(1, 1));
    mrb_define_class_method(mrb, mouse_module, "released?", moon_mrb_mouse_is_released, MRB_ARGS_ARG(1, 1));
    mrb_define_class_method(mrb, mouse_module, "modded?",   moon_mrb_mouse_is_modded,   MRB_ARGS_REQ(2));

    // input constants
    // C++11 range based for loop (friend function of Input to gain access to keyboard_keys)
    for (auto& i : Input::keyboard_keys) {
      auto& key = i.second;
      mrb_define_const(mrb, key_module, key.name, mrb_fixnum_value(key.button_id));
    };
    // key modifiers 
    mrb_define_const(mrb, key_module, "MOD_SHIFT",   mrb_fixnum_value(GLFW_MOD_SHIFT));
    mrb_define_const(mrb, key_module, "MOD_CONTROL", mrb_fixnum_value(GLFW_MOD_CONTROL));
    mrb_define_const(mrb, key_module, "MOD_ALT",     mrb_fixnum_value(GLFW_MOD_ALT));
    mrb_define_const(mrb, key_module, "MOD_SUPER",   mrb_fixnum_value(GLFW_MOD_SUPER));

    // mouse constants
    for (auto& i : Input::mouse_buttons) {
      auto& button = i.second;
      mrb_define_const(mrb, button_module, button.name, mrb_fixnum_value(button.button_id));
    };
    // alias constants
    mrb_define_const(mrb, button_module, "LEFT",   mrb_fixnum_value(Input::mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].button_id));
    mrb_define_const(mrb, button_module, "RIGHT",  mrb_fixnum_value(Input::mouse_buttons[GLFW_MOUSE_BUTTON_RIGHT].button_id));
    mrb_define_const(mrb, button_module, "MIDDLE", mrb_fixnum_value(Input::mouse_buttons[GLFW_MOUSE_BUTTON_MIDDLE].button_id));
  }

}
/* 
  mruby.api reference 
    void mrb_define_const(mrb_state*, struct RClass*, const char *name, mrb_value);
*/