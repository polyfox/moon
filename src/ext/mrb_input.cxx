#include "moon.hxx"
#include "input.hxx"

namespace Moon 
{ 

  static mrb_value moon_mrb_input_is_pressed(mrb_state *mrb, mrb_value rb_key_id) {
    return mrb_bool_value(Input::key_pressed(mrb_fixnum(rb_key_id)));
  }

  static mrb_value moon_mrb_input_is_released(mrb_state *mrb, mrb_value rb_key_id) {
    return mrb_bool_value(Input::key_released(mrb_fixnum(rb_key_id)));
  }

  void moon_mrb_input_init(mrb_state *mrb) {
    struct RClass *input_class;
    input_class = mrb_define_class(mrb, "Input", mrb->object_class);

    mrb_define_class_method(mrb, input_class, "pressed?", moon_mrb_input_is_pressed, MRB_ARGS_REQ(1));
    mrb_define_class_method(mrb, input_class, "released?", moon_mrb_input_is_released, MRB_ARGS_REQ(1));

    mrb_define_global_const(mrb, "Input", mrb_obj_value(input_class));
    for(int i = 0; i < 120; i++) {
      KeyboardKey* key = Input::get_key(i);
      mrb_define_const(mrb, input_class, key->name, mrb_fixnum_value((mrb_int)key->key));
    } 
  };

}
/* 
  mruby.api reference 
    void mrb_define_const(mrb_state*, struct RClass*, const char *name, mrb_value);
*/