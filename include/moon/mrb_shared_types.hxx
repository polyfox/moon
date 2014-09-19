#ifndef MOON_MRB_SHARED_TYPES_H
#define MOON_MRB_SHARED_TYPES_H

#include "moon/mrb.hxx"
#include "moon/shared_types.hxx"

namespace Moon {
  glm::vec2 moon_vector2_extract_args(mrb_state *mrb, int argc, mrb_value *args);
  glm::vec3 moon_vector3_extract_args(mrb_state *mrb, int argc, mrb_value *args);
  glm::vec4 moon_vector4_extract_args(mrb_state *mrb, int argc, mrb_value *args);

  inline glm::vec2
  moon_mrb_to_vec2(mrb_state *mrb, mrb_value obj) {
    mrb_value argv[1] = { obj };
    return moon_vector2_extract_args(mrb, 1, argv);
  }

  inline glm::vec3
  moon_mrb_to_vec3(mrb_state *mrb, mrb_value obj) {
    mrb_value argv[1] = { obj };
    return moon_vector3_extract_args(mrb, 1, argv);
  }

  inline glm::vec4
  moon_mrb_to_vec4(mrb_state *mrb, mrb_value obj) {
    mrb_value argv[1] = { obj };
    return moon_vector4_extract_args(mrb, 1, argv);
  }

  namespace mmrb {
    namespace Vector2 {
      mrb_value create(mrb_state *mrb, double x, double y);
      mrb_value wrap(mrb_state *mrb, glm::vec2 *ptr);
      mrb_value wrap(mrb_state *mrb, moon_vec2 moonv);
    }
    namespace Vector3 {
      mrb_value create(mrb_state *mrb, double x, double y, double z);
      mrb_value wrap(mrb_state *mrb, glm::vec3 *ptr);
      mrb_value wrap(mrb_state *mrb, moon_vec3 moonv);
    }
    namespace Vector4 {
      mrb_value create(mrb_state *mrb, double x, double y, double z, double w);
      mrb_value wrap(mrb_state *mrb, glm::vec4 *ptr);
      mrb_value wrap(mrb_state *mrb, moon_vec4 moonv);
    }
    namespace Rect {
      mrb_value create(mrb_state *mrb, GLint x, GLint y, GLint w, GLint h);
      mrb_value wrap(mrb_state *mrb, Moon::Rect *ptr);
      mrb_value wrap(mrb_state *mrb, moon_rect moonr);
    }
  }
};
#endif
