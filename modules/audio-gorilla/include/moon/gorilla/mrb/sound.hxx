#ifndef MMRB_SOUND_H
#define MMRB_SOUND_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"

MOON_C_API const struct mrb_data_type sound_data_type;
MOON_C_API void mmrb_sound_init(mrb_state *mrb);

#endif
