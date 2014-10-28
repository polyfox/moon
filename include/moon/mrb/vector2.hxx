#ifndef MMRB_VECTOR2_H
#define MMRB_VECTOR2_H

#include "moon/mrb.hxx"
#include "moon/vector2.hxx"

extern struct mrb_data_type vector2_data_type;
extern struct RClass* mmrb_Vector2;
struct RClass* mmrb_vector2_init(mrb_state *mrb);

glm::vec2 mmrb_vector2_extract_mrb_vec2(mrb_state *mrb, mrb_value obj);
mrb_value mmrb_vector2_create(mrb_state *mrb, double x, double y, double z, double w);
mrb_value mmrb_vector2_wrap(mrb_state *mrb, glm::vec2 *ptr);
mrb_value mmrb_vector2_wrap(mrb_state *mrb, moon_vec2 moonv);

#endif
