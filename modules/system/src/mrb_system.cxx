#include <stdbool.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/engine.hxx"
#include "moon/mrb/clock.hxx"
#include "moon/mrb/context.hxx"
#include "moon/mrb/rect.hxx"
#include "moon/mrb/matrix4.hxx"
#include "moon/mrb/vector1.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"

extern "C" void
mrb_mruby_moon_system_gem_init(mrb_state* mrb)
{
  struct RClass *moon_module = mrb_define_module(mrb, "Moon");
  mmrb_clock_init(mrb, moon_module);
  mmrb_rect_init(mrb, moon_module);
  mmrb_vector1_init(mrb, moon_module);
  mmrb_vector2_init(mrb, moon_module);
  mmrb_vector3_init(mrb, moon_module);
  mmrb_vector4_init(mrb, moon_module);
  mmrb_matrix4_init(mrb, moon_module);
  mmrb_context_init(mrb, moon_module);
}

extern "C" void
mrb_mruby_moon_system_gem_final(mrb_state* mrb)
{
  //
}
