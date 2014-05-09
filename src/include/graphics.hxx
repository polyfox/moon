#ifndef MOON_GFX_H
#define MOON_GFX_H

#include "moon.hxx"
#include <glm/glm.hpp>

namespace Moon {

  struct Vec2 {
    GLfloat x;
    GLfloat y;
    /*union {
      struct {
        GLfloat x;
        GLfloat y;
      };
      struct {
        GLfloat u;
        GLfloat v;
      };
    };*/
  };

  struct Vec3 {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    /*
    union {
      struct {
        GLfloat x;
        GLfloat y;
        GLfloat z;
      };
      struct {
        GLfloat r;
        GLfloat g;
        GLfloat b;
      };
      struct {
        vec2 rg;
        GLfloat b_;
      };
      struct {
        GLfloat r_;
        vec2 gb;
      };
    };*/
  };

  struct Vec4 {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
    /*
    union {
      struct {
        GLfloat r;
        GLfloat g;
        GLfloat b;
        GLfloat a;
      };
      struct {
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLfloat w;
      };
      struct {
        vec2 rg;
        vec2 ba;
      };
      struct {
        vec2 xy;
        vec2 zw;
      };
      struct {
        vec2 uv;
        vec2 zw_;
      };
      struct {
        vec3 rgb;
        GLfloat a_;
      };
      struct {
        vec3 xyz;
        GLfloat w_;
      };
    };*/
  };

  struct UVmap {
    GLfloat u;
    GLfloat v;
  };

  struct Color {
    Color() = default;
    Color(glm::vec4 v4) {
      r = (GLfloat)v4.r;
      g = (GLfloat)v4.g;
      b = (GLfloat)v4.b;
      a = (GLfloat)v4.a;
    };
    Color(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a) {
      r = _r;
      g = _g;
      b = _b;
      a = _a;
    };
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
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

  struct vertex {
    Vec2 pos;
    UVmap tex_coord;
    Color color;
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