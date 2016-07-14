#ifndef MMRB_VECTOR3_H
#define MMRB_VECTOR3_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/vector3.hxx"
#include "moon/mrb/helpers.hxx"

MOON_C_API const struct mrb_data_type vector3_data_type;
MOON_C_API void mmrb_vector3_init(mrb_state *mrb);
MOON_C_API Moon::Vector3 mmrb_to_vector3(mrb_state *mrb, mrb_value obj);
MOON_C_API mrb_value mmrb_vector3_value(mrb_state *mrb, Moon::Vector3 v3);

static inline struct RClass*
mmrb_get_vector3_class(mrb_state *mrb)
{
  return MOON_GET_CLASS("Vector3");
}

static inline Moon::Vector3*
mmrb_vector3_ptr(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Vector3*>(mrb_data_get_ptr(mrb, self, &vector3_data_type));
}

#endif
