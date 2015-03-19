#ifndef MMRB_RECT_H
#define MMRB_RECT_H

#include <memory.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/rect.hxx"

MOON_C_API const struct mrb_data_type rect_data_type;
MOON_C_API void mmrb_rect_init(mrb_state *mrb, struct RClass *mod);
MOON_C_API mrb_value mmrb_rect_value(mrb_state *mrb, Moon::IntRect rect);
MOON_C_API Moon::IntRect mmrb_to_rect(mrb_state *mrb, mrb_value obj);

#endif
