#ifndef MOON_GFX_H
#define MOON_GFX_H

#include "moon.hxx"
#include <glm/glm.hpp>

namespace Moon {

  struct vertex {
    glm::vec2 pos;
    glm::vec2 tex_coord;
    glm::vec4 color;
  };

  struct Rect {
    Rect() = default;
    Rect(GLint x, GLint y, GLint w, GLint h)  {
      this->x = x;
      this->y = y;
      this->w = w;
      this->h = h;
    }
    GLint x;
    GLint y;
    GLint w;
    GLint h;
  };

  struct ss_render_options {
    float opacity;
    float ox;
    float oy;
    float angle;
    glm::vec4 color;
    glm::vec4 tone;
    glm::mat4 transform;
  };

  struct font_render_options {
    float outline;
    glm::vec4 color;
    glm::vec4 outline_color;
    glm::mat4 transform;
    font_render_options() :
      outline(0.0),
      color(1.0, 1.0, 1.0, 1.0),
      outline_color(0.0, 0.0, 0.0, 0.9)
    {
    };
  };
}
#endif