#ifndef MMRB_VECTOR2_H
#define MMRB_VECTOR2_H

// For shared_ptr
#include <memory>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/vector2.hxx"

extern const struct mrb_data_type vector2_data_type;
void mmrb_vector2_init(mrb_state *mrb, struct RClass *mod);
glm::vec2 mmrb_to_vector2(mrb_state *mrb, mrb_value obj);
mrb_value mmrb_vector2_value(mrb_state *mrb, glm::vec2 v2);

#endif
