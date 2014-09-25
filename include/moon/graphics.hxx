#ifndef MOON_GFX_H
#define MOON_GFX_H

#include "moon/stdh.hxx"
#include <glm/glm.hpp>

namespace Moon {
  struct vertex {
    glm::vec2 pos;
    glm::vec2 tex_coord;
    glm::vec4 color;
  };
}
#endif
