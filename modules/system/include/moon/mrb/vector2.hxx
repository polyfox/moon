#ifndef MMRB_VECTOR2_H
#define MMRB_VECTOR2_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/vector2.hxx"
#include "moon/mrb/helpers.hxx"

MOON_C_API const struct mrb_data_type vector2_data_type;
MOON_C_API void mmrb_vector2_init(mrb_state *mrb);
MOON_C_API Moon::Vector2 mmrb_to_vector2(mrb_state *mrb, mrb_value obj);
MOON_C_API mrb_value mmrb_vector2_value(mrb_state *mrb, Moon::Vector2 v2);

static inline struct RClass*
mmrb_get_vector2_class(mrb_state *mrb)
{
  return MOON_GET_CLASS("Vector2");
}

static inline Moon::Vector2*
mmrb_vector2_ptr(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Vector2*>(mrb_data_get_ptr(mrb, self, &vector2_data_type));
}

#endif
