#ifndef MMRB_TRANSFORM_H
#define MMRB_TRANSFORM_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/transform.hxx"

MOON_C_API const struct mrb_data_type transform_data_type;
MOON_C_API void mmrb_transform_init(mrb_state *mrb, struct RClass *mod);
MOON_C_API mrb_value mmrb_transform_value(mrb_state *mrb, Moon::Transform mat);
MOON_C_API Moon::Transform mmrb_to_transform(mrb_state *mrb, mrb_value self);

#endif
