#ifndef MMRB_MUSIC_H
#define MMRB_MUSIC_H

#include "moon/api.h"
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>

MOON_C_API const struct mrb_data_type music_data_type;
MOON_C_API void mmrb_music_init(mrb_state* mrb);

#endif
