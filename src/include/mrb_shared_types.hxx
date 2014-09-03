#ifndef MOON_MRB_SHARED_TYPES_H
#define MOON_MRB_SHARED_TYPES_H

#include <mrb.hxx>
#include "shared_types.hxx"

namespace Moon {
  glm::vec2 moon_vector2_extract_args(mrb_state *mrb, int argc, mrb_value *args);
  glm::vec3 moon_vector3_extract_args(mrb_state *mrb, int argc, mrb_value *args);
  glm::vec4 moon_vector4_extract_args(mrb_state *mrb, int argc, mrb_value *args);

  inline glm::vec2
  moon_mrb_to_vec2(mrb_state* mrb, mrb_value obj) {
    mrb_value argv[1] = { obj };
    return moon_vector2_extract_args(mrb, 1, argv);
  }

  inline glm::vec3
  moon_mrb_to_vec3(mrb_state* mrb, mrb_value obj) {
    mrb_value argv[1] = { obj };
    return moon_vector3_extract_args(mrb, 1, argv);
  }

  inline glm::vec4
  moon_mrb_to_vec4(mrb_state* mrb, mrb_value obj) {
    mrb_value argv[1] = { obj };
    return moon_vector4_extract_args(mrb, 1, argv);
  }
};
#endif
