#ifndef MMRB_VECTOR1_H
#define MMRB_VECTOR1_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/vector1.hxx"

extern const struct mrb_data_type vector1_data_type;
void mmrb_vector1_init(mrb_state *mrb, struct RClass *mod);
Moon::Vector1 mmrb_to_vector1(mrb_state *mrb, mrb_value obj);
mrb_value mmrb_vector1_value(mrb_state *mrb, Moon::Vector1 v1);

static inline mrb_float
mmrb_to_flo(mrb_state *mrb, mrb_value obj)
{
  if (mrb_type(obj) == MRB_TT_DATA) {
    if (DATA_TYPE(obj) == &vector1_data_type) {
      return mmrb_to_vector1(mrb, obj)[0];
    }
  }
  return mrb_to_flo(mrb, obj);
}

#endif
