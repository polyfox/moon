#include "mrb.hxx"

mrb_value hola_hi(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, "Hola!");
}

void init_hola(mrb_state *mrb)
{
  struct RClass *hola;
  hola = mrb_define_module(mrb, "Hola");

  mrb_define_class_method(mrb, hola, "hi", hola_hi, MRB_ARGS_NONE());
  //mrb_value hola = mrb_class_get(engine->mrb, "Hola")
  //mrb_funcall(engine->mrb, hola, "hi", 0);
}