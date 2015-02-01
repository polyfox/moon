#ifndef MMRB_VECTOR1_H
#define MMRB_VECTOR1_H

#include <memory>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/vector1.hxx"

extern const struct mrb_data_type vector1_data_type;
void mmrb_vector1_init(mrb_state *mrb, struct RClass *mod);
glm::highp_vec1_t mmrb_to_vector1(mrb_state *mrb, mrb_value obj);
mrb_value mmrb_vector1_value(mrb_state *mrb, glm::highp_vec1_t v1);

#endif
