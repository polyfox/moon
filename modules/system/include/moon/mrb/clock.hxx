#ifndef MMRB_CLOCK_H
#define MMRB_CLOCK_H

#include <memory.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"

MOON_C_API const struct mrb_data_type clock_data_type;
MOON_C_API void mmrb_clock_init(mrb_state *mrb, struct RClass *mod);

#endif
