#ifndef MMRB_VBO_H
#define MMRB_VBO_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"

MOON_C_API const struct mrb_data_type vbo_data_type;
MOON_C_API void mmrb_vbo_init(mrb_state *mrb, struct RClass* mod);

#endif
