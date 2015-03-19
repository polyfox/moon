#ifndef MMRB_SHADER_H
#define MMRB_SHADER_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"

MOON_C_API void mmrb_shader_init(mrb_state *mrb, struct RClass* mod);

#endif
