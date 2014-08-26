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
  moon_mrb_mouse_position(mrb_state *mrb, mrb_value self) {
    auto pos = Input::Mouse::get_position();

    mrb_value argv[2] = { mrb_fixnum_value(pos[0]), mrb_fixnum_value(pos[1]) };
    return mrb_obj_new(mrb, moon_cVector2, 2, argv);
  }

  struct RClass*
  moon_mrb_input_init(mrb_state *mrb) {
    struct RClass *input_module;
    struct RClass *mouse_module;

    input_module = mrb_define_module_under(mrb, moon_module, "Input");
    mouse_module = mrb_define_module_under(mrb, input_module, "Mouse");

    // mouse functions
    mrb_define_class_method(mrb, mouse_module, "x",        moon_mrb_mouse_x,        MRB_ARGS_NONE());
    mrb_define_class_method(mrb, mouse_module, "y",        moon_mrb_mouse_y,        MRB_ARGS_NONE());
    mrb_define_class_method(mrb, mouse_module, "position", moon_mrb_mouse_position, MRB_ARGS_NONE());
    return input_module;
  }
}
/*
  mruby.api reference
    void mrb_define_const(mrb_state*, struct RClass*, const char *name, mrb_value);
*/
