#ifndef MMRB_VECTOR4_H
#define MMRB_VECTOR4_H

#include "moon/mrb.hxx"
#include "moon/vector4.hxx"

extern struct mrb_data_type vector4_data_type;
extern struct RClass* mmrb_Vector4;
struct RClass* mmrb_vector4_init(mrb_state *mrb);

glm::vec4 mmrb_vector4_extract_mrb_vec4(mrb_state *mrb, mrb_value obj);
mrb_value mmrb_vector4_create(mrb_state *mrb, double x, double y, double z, double w);
mrb_value mmrb_vector4_wrap(mrb_state *mrb, glm::vec4 *ptr);
mrb_value mmrb_vector4_wrap(mrb_state *mrb, moon_vec4 moonv);

#endif
