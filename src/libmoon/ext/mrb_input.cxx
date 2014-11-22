#include "moon/mrb/input.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/input.hxx"

using Moon::Input;

static mrb_value
mouse_x(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(Input::Mouse::x());
}

static mrb_value
mouse_y(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(Input::Mouse::y());
}

static mrb_value
mouse_position(mrb_state *mrb, mrb_value self)
{
  auto pos = Input::Mouse::get_position();

  mrb_value argv[2] = { mrb_fixnum_value(pos[0]), mrb_fixnum_value(pos[1]) };
  return mrb_obj_new(mrb, mmrb_Vector2, 2, argv);
}

struct RClass*
mmrb_input_init(mrb_state *mrb)
{
  struct RClass *input_module;
  struct RClass *mouse_module;

  input_module = mrb_define_module_under(mrb, mmrb_Moon, "Input");
  mouse_module = mrb_define_module_under(mrb, input_module, "Mouse");

  // mouse functions
  mrb_define_class_method(mrb, mouse_module, "x",        mouse_x,        MRB_ARGS_NONE());
  mrb_define_class_method(mrb, mouse_module, "y",        mouse_y,        MRB_ARGS_NONE());
  mrb_define_class_method(mrb, mouse_module, "position", mouse_position, MRB_ARGS_NONE());

  mrb_define_const(mrb, input_module, "MOD_SHIFT",   mrb_fixnum_value(GLFW_MOD_SHIFT));
  mrb_define_const(mrb, input_module, "MOD_CONTROL", mrb_fixnum_value(GLFW_MOD_CONTROL));
  mrb_define_const(mrb, input_module, "MOD_ALT",     mrb_fixnum_value(GLFW_MOD_ALT));
  mrb_define_const(mrb, input_module, "MOD_SUPER",   mrb_fixnum_value(GLFW_MOD_SUPER));

  return input_module;
}
