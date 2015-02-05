#include <mruby.h>
#include <mruby/class.h>
#include "moon/mrb/context.hxx"
#include "moon/mrb/rect.hxx"
#include "moon/mrb/transform.hxx"
#include "moon/mrb/vector1.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"

static struct RClass *moon_module;

extern "C" void
mrb_mruby_moon_system_gem_init(mrb_state* mrb)
{
  moon_module = mrb_define_module(mrb, "Moon");
  mmrb_rect_init(mrb, moon_module);
  mmrb_vector1_init(mrb, moon_module);
  mmrb_vector2_init(mrb, moon_module);
  mmrb_vector3_init(mrb, moon_module);
  mmrb_vector4_init(mrb, moon_module);
  mmrb_transform_init(mrb, moon_module);
  mmrb_context_init(mrb, moon_module);
}

extern "C" void
mrb_mruby_moon_system_gem_final(mrb_state* mrb)
{
  //
}
