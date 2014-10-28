#ifndef MMRB_VECTOR3_H
#define MMRB_VECTOR3_H

#include "moon/mrb.hxx"
#include "moon/vector3.hxx"

extern struct mrb_data_type vector3_data_type;
extern struct RClass* mmrb_Vector3;
struct RClass* mmrb_vector3_init(mrb_state *mrb);

glm::vec3 mmrb_vector3_extract_mrb_vec3(mrb_state *mrb, mrb_value obj);
mrb_value mmrb_vector3_create(mrb_state *mrb, double x, double y, double z, double w);
mrb_value mmrb_vector3_wrap(mrb_state *mrb, glm::vec3 *ptr);
mrb_value mmrb_vector3_wrap(mrb_state *mrb, moon_vec3 moonv);

#endif
