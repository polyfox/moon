#ifndef MOON_GFX_H
#define MOON_GFX_H

#include "moon/vector2.hxx"
#include "moon/vector4.hxx"

namespace Moon {
  struct Vertex {
    Vector2 pos;
    Vector2 tex_coord;
    Vector4 color;
  };
}
#endif
