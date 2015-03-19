#ifndef MMRB_TEXTURE_H
#define MMRB_TEXTURE_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"

MOON_C_API const struct mrb_data_type texture_data_type;
MOON_C_API void mmrb_texture_init(mrb_state *mrb, struct RClass* mod);
MOON_C_API mrb_value mmrb_texture_load_file(mrb_state *mrb, const char *filename);

#endif
