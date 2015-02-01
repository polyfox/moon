#ifndef MMRB_VECTOR4_H
#define MMRB_VECTOR4_H

#include <memory>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/vector4.hxx"

extern const struct mrb_data_type vector4_data_type;
void mmrb_vector4_init(mrb_state *mrb, struct RClass *mod);
glm::vec4 mmrb_to_vector4(mrb_state *mrb, mrb_value obj);
mrb_value mmrb_vector4_value(mrb_state *mrb, glm::vec4 v4);

#endif
