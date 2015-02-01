#ifndef MMRB_VECTOR3_H
#define MMRB_VECTOR3_H

#include <memory>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/vector3.hxx"

extern const struct mrb_data_type vector3_data_type;
void mmrb_vector3_init(mrb_state *mrb, struct RClass *mod);
Moon::Vector3 mmrb_to_vector3(mrb_state *mrb, mrb_value obj);
mrb_value mmrb_vector3_value(mrb_state *mrb, Moon::Vector3 v3);

#endif
