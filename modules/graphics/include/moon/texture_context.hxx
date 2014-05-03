#ifndef MOON_TEXTURE_CONTEXT_H
#define MOON_TEXTURE_CONTEXT_H

#include <memory>
#include <SOIL.h>
#include "moon/intern.h"
#include "moon/gl.h"
#include "moon/texture.h"
#include "moon/vector4.h"

namespace Moon {
  class TextureContext {
  public:
    Texture* texture;

    TextureContext(Texture* _texture);
    Vector4 get_pixel(const int x, const int y);
    void set_pixel(const int x, const int y, const Vector4 &color);
    void fill_rect(const Rect &rect, const Vector4 &color);
  };
}

#endif
