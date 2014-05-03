#include "moon/texture.hxx"
#include "moon/vector4.hxx"
#include "moon/rect.hxx"

namespace Moon {
  TextureContext::TextureContext(Texture* _texture) :
    texture(_texture)
  {
  }

  Vector4 TextureContext::get_pixel(const int x, const int y)
  {
    return Vector4(1.0, 1.0, 1.0, 1.0); // placeholder
  }

  void TextureContext::set_pixel(const int x, const int y, const Vector4 &color)
  {
    // works some pixelly magic
  }

  void TextureContext::fill_rect(const IntRect &rect, const Vector4 &color)
  {
    // works some rectangle magic
  }
}
