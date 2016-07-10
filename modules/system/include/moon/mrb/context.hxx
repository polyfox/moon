#ifndef MMRB_CONTEXT_H
#define MMRB_CONTEXT_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"

MOON_C_API const struct mrb_data_type context_data_type;
MOON_C_API void mmrb_context_init(mrb_state *mrb);

#endif
