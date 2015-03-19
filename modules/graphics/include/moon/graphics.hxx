#ifndef MOON_GFX_H
#define MOON_GFX_H

#include "moon/vector2.hxx"
#include "moon/vector4.hxx"

namespace Moon {
  struct Vertex {
    Moon::Vector2 pos;
    Moon::Vector2 tex_coord;
    Moon::Vector4 color;
  };
}
#endif
