#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/mrb/clock.hxx"
#include "moon/fps.hxx"

static void
clock_free(mrb_state *mrb, void *ptr)
{
  Moon::FPS *clk = (Moon::FPS*)ptr;
  if (clk) {
    delete(clk);
  }
}

MOON_C_API const mrb_data_type clock_data_type = { "Moon::FPS", clock_free };

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

// @return [self]
static mrb_value
clock_initialize_copy(mrb_state *mrb, mrb_value self)
{
  clock_cleanup(mrb, self);
  mrb_data_init(self, new Moon::FPS(), &clock_data_type);
  return self;
}

// @return [Float]
static mrb_value
clock_restart(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_clock_ptr(mrb, self)->Restart());
}

// @return [self]
static mrb_value
clock_update(mrb_state *mrb, mrb_value self)
{
  mmrb_clock_ptr(mrb, self)->Update();
  return self;
}

// @return [Float]
static mrb_value
clock_delta(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_clock_ptr(mrb, self)->GetDelta());
}

// @return [Integer]
static mrb_value
clock_fps(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mmrb_clock_ptr(mrb, self)->GetFPS());
}

MOON_C_API void
mmrb_clock_init(mrb_state *mrb, struct RClass *mod)
{
  // Utility class for tracking time.
  struct RClass *clock_class = mrb_define_class_under(mrb, mod, "Clock", mrb->object_class);
  MRB_SET_INSTANCE_TT(clock_class, MRB_TT_DATA);

  mrb_define_method(mrb, clock_class, "initialize", clock_initialize,           MRB_ARGS_NONE());
  mrb_define_method(mrb, clock_class, "initialize_copy", clock_initialize_copy, MRB_ARGS_NONE());
  mrb_define_method(mrb, clock_class, "restart",    clock_restart,              MRB_ARGS_NONE());
  mrb_define_method(mrb, clock_class, "update",     clock_update,               MRB_ARGS_NONE());
  mrb_define_method(mrb, clock_class, "delta",      clock_delta,                MRB_ARGS_NONE());
  mrb_define_method(mrb, clock_class, "fps",        clock_fps,                  MRB_ARGS_NONE());
}
