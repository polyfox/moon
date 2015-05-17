#ifndef MMRB_VECTOR4_H
#define MMRB_VECTOR4_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/vector4.hxx"


MOON_C_API const struct mrb_data_type vector4_data_type;
MOON_C_API void mmrb_vector4_init(mrb_state *mrb, struct RClass *mod);
MOON_C_API Moon::Vector4 mmrb_to_vector4(mrb_state *mrb, mrb_value obj);
MOON_C_API mrb_value mmrb_vector4_value(mrb_state *mrb, Moon::Vector4 v4);

static inline Moon::Vector4*
mmrb_vector4_ptr(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Vector4*>(mrb_data_get_ptr(mrb, self, &vector4_data_type));
}

#endif
