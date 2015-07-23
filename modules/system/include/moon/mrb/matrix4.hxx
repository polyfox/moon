#ifndef MMRB_TRANSFORM_H
#define MMRB_TRANSFORM_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/matrix4.hxx"

MOON_C_API const struct mrb_data_type matrix4_data_type;
MOON_C_API void mmrb_matrix4_init(mrb_state *mrb, struct RClass *mod);
MOON_C_API mrb_value mmrb_matrix4_value(mrb_state *mrb, Moon::Matrix4 mat);
MOON_C_API Moon::Matrix4 mmrb_to_matrix4(mrb_state *mrb, mrb_value self);

#endif
