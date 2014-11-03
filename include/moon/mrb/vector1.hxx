#ifndef MMRB_VECTOR1_H
#define MMRB_VECTOR1_H

#include "moon/mrb.hxx"
#include "moon/vector1.hxx"

extern struct mrb_data_type vector1_data_type;
extern struct RClass* mmrb_Vector1;
struct RClass* mmrb_vector1_init(mrb_state *mrb);

glm::highp_vec1_t mmrb_vector1_extract_mrb_vec1(mrb_state *mrb, mrb_value obj);
mrb_value mmrb_vector1_create(mrb_state *mrb, double x);
mrb_value mmrb_vector1_wrap(mrb_state *mrb, glm::highp_vec1_t *ptr);
mrb_value mmrb_vector1_wrap(mrb_state *mrb, moon_vec1 moonv);

#endif
