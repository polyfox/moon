#ifndef MMRB_VECTOR2_H
#define MMRB_VECTOR2_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/vector2.hxx"


MOON_C_API const struct mrb_data_type vector2_data_type;
MOON_C_API void mmrb_vector2_init(mrb_state *mrb, struct RClass *mod);
MOON_C_API Moon::Vector2 mmrb_to_vector2(mrb_state *mrb, mrb_value obj);
MOON_C_API mrb_value mmrb_vector2_value(mrb_state *mrb, Moon::Vector2 v2);

#endif
