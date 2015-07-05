#ifndef MMRB_TEXT_H
#define MMRB_TEXT_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"

MOON_C_API void mmrb_text_init(mrb_state *mrb, struct RClass* mod);

#endif
