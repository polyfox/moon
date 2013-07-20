#include "moon.hxx"
#include "input.hxx"

namespace Moon 
{ 

  static mrb_value moon_mrb_input_is_pressed(mrb_state *mrb, mrb_value self) {
    mrb_int key_id;
    mrb_get_args(mrb, "i", &key_id);
    return mrb_bool_value(Input::key_pressed(key_id));
  }

  static mrb_value moon_mrb_input_is_released(mrb_state *mrb, mrb_value self) {
    mrb_int key_id;
    mrb_get_args(mrb, "i", &key_id);
    return mrb_bool_value(Input::key_released(key_id));
  }

  static mrb_value moon_mrb_input_is_repeated(mrb_state *mrb, mrb_value self) {
    mrb_int key_id;
    mrb_get_args(mrb, "i", &key_id);
    return mrb_bool_value(Input::key_repeated(key_id));
  }

  static mrb_value moon_mrb_mouse_x(mrb_state *mrb, mrb_value self) {
    return mrb_fixnum_value(Input::Mouse::x());
  };

  static mrb_value moon_mrb_mouse_y(mrb_state *mrb, mrb_value self) {
    return mrb_fixnum_value(Input::Mouse::y());
  };

  static mrb_value moon_mrb_mouse_is_pressed(mrb_state *mrb, mrb_value self) {
    mrb_int button_id;
    mrb_get_args(mrb, "i", &button_id);
    return mrb_bool_value(Input::Mouse::button_pressed(button_id));
  };

  static mrb_value moon_mrb_mouse_is_released(mrb_state *mrb, mrb_value self) {
    mrb_int button_id;
    mrb_get_args(mrb, "i", &button_id);
    return mrb_bool_value(Input::Mouse::button_released(button_id));
  };

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
    mrb_define_class_method(mrb, input_class, "pressed?", moon_mrb_input_is_pressed, MRB_ARGS_REQ(1));
    mrb_define_class_method(mrb, input_class, "released?", moon_mrb_input_is_released, MRB_ARGS_REQ(1));
    mrb_define_class_method(mrb, input_class, "repeated?", moon_mrb_input_is_repeated, MRB_ARGS_REQ(1));

    // mouse functions
    mrb_define_class_method(mrb, mouse_module, "x", moon_mrb_mouse_x, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, mouse_module, "y", moon_mrb_mouse_y, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, mouse_module, "pressed?", moon_mrb_mouse_is_pressed, MRB_ARGS_REQ(1));
    mrb_define_class_method(mrb, mouse_module, "released?", moon_mrb_mouse_is_released, MRB_ARGS_REQ(1));

    // input constants
    // C++11 range based for loop (friend function of Input to gain access to keyboard_keys)
    for (auto& i : Input::keyboard_keys) {
      auto& key = i.second;
      mrb_define_const(mrb, key_module, key.name, mrb_fixnum_value(key.button_id));
    };

    // mouse constants
    for (auto& i : Input::mouse_buttons) {
      auto& button = i.second;
      mrb_define_const(mrb, button_module, button.name, mrb_fixnum_value(button.button_id));
    };
    // alias constants
    mrb_define_const(mrb, button_module, "LEFT", mrb_fixnum_value(Input::mouse_buttons[GLFW_MOUSE_BUTTON_LEFT].button_id));
    mrb_define_const(mrb, button_module, "RIGHT", mrb_fixnum_value(Input::mouse_buttons[GLFW_MOUSE_BUTTON_RIGHT].button_id));
    mrb_define_const(mrb, button_module, "MIDDLE", mrb_fixnum_value(Input::mouse_buttons[GLFW_MOUSE_BUTTON_MIDDLE].button_id));
  };

}
/* 
  mruby.api reference 
    void mrb_define_const(mrb_state*, struct RClass*, const char *name, mrb_value);
*/