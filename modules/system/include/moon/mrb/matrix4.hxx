#ifndef MMRB_TRANSFORM_H
#define MMRB_TRANSFORM_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/matrix4.hxx"
#include "moon/mrb/helpers.hxx"

MOON_C_API const struct mrb_data_type matrix4_data_type;
MOON_C_API void mmrb_matrix4_init(mrb_state *mrb);
MOON_C_API mrb_value mmrb_matrix4_value(mrb_state *mrb, Moon::Matrix4 mat);
MOON_C_API Moon::Matrix4 mmrb_to_matrix4(mrb_state *mrb, mrb_value self);

static inline struct RClass*
mmrb_get_matrix4_class(mrb_state *mrb)
{
  return MOON_GET_CLASS("Matrix4");
}

static inline Moon::Matrix4*
mmrb_matrix4_ptr(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Matrix4*>(mrb_data_get_ptr(mrb, self, &matrix4_data_type));
}

#endif
