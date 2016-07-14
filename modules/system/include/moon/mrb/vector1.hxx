#ifndef MMRB_VECTOR1_H
#define MMRB_VECTOR1_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/api.h"
#include "moon/vector1.hxx"
#include "moon/mrb/helpers.hxx"

MOON_C_API const struct mrb_data_type vector1_data_type;
MOON_C_API void mmrb_vector1_init(mrb_state *mrb);
MOON_C_API Moon::Vector1 mmrb_to_vector1(mrb_state *mrb, mrb_value obj);
MOON_C_API mrb_value mmrb_vector1_value(mrb_state *mrb, Moon::Vector1 v1);

static inline struct RClass*
mmrb_get_vector1_class(mrb_state *mrb)
{
  return MOON_GET_CLASS("Vector1");
}

static inline Moon::Vector1*
mmrb_vector1_ptr(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Vector1*>(mrb_data_get_ptr(mrb, self, &vector1_data_type));
}

static inline mrb_float
mmrb_to_flo(mrb_state *mrb, mrb_value obj)
{
  if (mrb_type(obj) == MRB_TT_DATA) {
    if (DATA_TYPE(obj) == &vector1_data_type) {
      return (*mmrb_vector1_ptr(mrb, obj))[0];
    }
  }
  return mrb_to_flo(mrb, obj);
}

#endif
