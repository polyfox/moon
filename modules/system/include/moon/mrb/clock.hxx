#ifndef MMRB_CLOCK_H
#define MMRB_CLOCK_H

#include <memory.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/fps.hxx"

MOON_C_API const struct mrb_data_type clock_data_type;
MOON_C_API void mmrb_clock_init(mrb_state *mrb);

static inline Moon::FPS*
mmrb_clock_ptr(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::FPS*>(mrb_data_get_ptr(mrb, self, &clock_data_type));
}

#endif
