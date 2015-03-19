#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/mrb/clock.hxx"
#include "moon/fps.hxx"

static struct RClass *clock_class;

static void
clock_free(mrb_state *mrb, void *ptr)
{
  Moon::FPS *clk = (Moon::FPS*)ptr;
  if (clk) {
    delete(clk);
  }
}

MOON_C_API const mrb_data_type clock_data_type = { "Moon::FPS", clock_free };

static inline Moon::FPS*
get_clock(mrb_state *mrb, mrb_value self)
{
  return (Moon::FPS*)mrb_data_get_ptr(mrb, self, &clock_data_type);
}

static inline void
clock_cleanup(mrb_state *mrb, mrb_value self)
{
  void *ptr = DATA_PTR(self);
  if (ptr) {
    clock_free(mrb, ptr);
  }
  DATA_PTR(self) = NULL;
}

static mrb_value
clock_initialize(mrb_state *mrb, mrb_value self)
{
  clock_cleanup(mrb, self);
  mrb_data_init(self, new Moon::FPS(), &clock_data_type);
  return self;
}

static mrb_value
clock_initialize_copy(mrb_state *mrb, mrb_value self)
{
  clock_cleanup(mrb, self);
  mrb_data_init(self, new Moon::FPS(), &clock_data_type);
  return self;
}

static mrb_value
clock_restart(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_clock(mrb, self)->restart());
}

static mrb_value
clock_update(mrb_state *mrb, mrb_value self)
{
  get_clock(mrb, self)->update();
  return self;
}

static mrb_value
clock_delta(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_clock(mrb, self)->getDelta());
}

static mrb_value
clock_fps(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_clock(mrb, self)->getFPS());
}

MOON_C_API void
mmrb_clock_init(mrb_state *mrb, struct RClass *mod)
{
  clock_class = mrb_define_class_under(mrb, mod, "Clock", mrb->object_class);
  MRB_SET_INSTANCE_TT(clock_class, MRB_TT_DATA);

  mrb_define_method(mrb, clock_class, "initialize", clock_initialize,           MRB_ARGS_NONE());
  mrb_define_method(mrb, clock_class, "initialize_copy", clock_initialize_copy, MRB_ARGS_NONE());
  mrb_define_method(mrb, clock_class, "restart",    clock_restart,              MRB_ARGS_NONE());
  mrb_define_method(mrb, clock_class, "update",     clock_update,               MRB_ARGS_NONE());
  mrb_define_method(mrb, clock_class, "delta",      clock_delta,                MRB_ARGS_NONE());
  mrb_define_method(mrb, clock_class, "fps",        clock_fps,                  MRB_ARGS_NONE());
}
