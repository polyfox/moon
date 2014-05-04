#ifndef MOON_GFX_H
#define MOON_GFX_H

#include "moon.hxx"

namespace Moon {
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

  struct vec2 {
    GLfloat x;
    GLfloat y;
  };

  struct uvMap {
    GLfloat u;
    GLfloat v;
  };

  struct Color {
    Color() = default;
    Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
      this->r = r;
      this->g = g;
      this->b = b;
      this->a = a;
    };
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
  };

  struct vertex {
    vec2  pos;
    uvMap tex_coord;
    Color color;
  };

  struct render_options {
    float opacity;
    float ox;
    float oy;
    float angle;
    Color color;
    Color tone;
  };

  struct font_render_options {
    float outline;
    Color color;
    Color outline_color;
    font_render_options() :
      outline(0.0),
      color(1.0, 1.0, 1.0, 1.0),
      outline_color(0.0, 0.0, 0.0, 0.9)
    {
    };
  };
}
#endif