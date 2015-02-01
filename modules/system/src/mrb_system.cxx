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

void
mrb_mruby_moon_system_gem_init(mrb_state* mrb)
{
  moon_module = mrb_define_module(mrb, "Moon");
}
