#ifndef MOON_MRB_SHARED_TYPES_H
#define MOON_MRB_SHARED_TYPES_H

#include "moon/mrb.hxx"
#include "moon/shared_types.hxx"

glm::vec2 mmrb_vector2_extract_args(mrb_state *mrb, int argc, mrb_value *args);
glm::vec3 mmrb_vector3_extract_args(mrb_state *mrb, int argc, mrb_value *args);
glm::vec4 mmrb_vector4_extract_args(mrb_state *mrb, int argc, mrb_value *args);

inline glm::vec2
mmrb_obj_to_vec2(mrb_state *mrb, mrb_value obj) {
  mrb_value argv[1] = { obj };
  return mmrb_vector2_extract_args(mrb, 1, argv);
}

inline glm::vec3
mmrb_obj_to_vec3(mrb_state *mrb, mrb_value obj) {
  mrb_value argv[1] = { obj };
  return mmrb_vector3_extract_args(mrb, 1, argv);
}

inline glm::vec4
mmrb_obj_to_vec4(mrb_state *mrb, mrb_value obj) {
  mrb_value argv[1] = { obj };
  return mmrb_vector4_extract_args(mrb, 1, argv);
}
#endif
