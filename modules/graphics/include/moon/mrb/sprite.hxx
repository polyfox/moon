#ifndef MMRB_SPRITE_H
#define MMRB_SPRITE_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"

MOON_C_API const struct mrb_data_type sprite_data_type;
MOON_C_API void mmrb_sprite_init(mrb_state *mrb, struct RClass* mod);

#endif
