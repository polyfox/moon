#ifndef MOON_FONT_H
#define MOON_FONT_H

#include <freetype-gl.h>
#include "moon/glm.h"
#include "moon/intern.h"
#include "moon/shader.hxx"
#include "moon/vertex_buffer.hxx"
#include "moon/vector2.hxx"
#include "moon/vector4.hxx"
#include "moon/string.hxx"

namespace Moon {
  class Font {
  public:
    texture_font_t *font;
    texture_atlas_t *atlas;

    Font(const std::string &name, const int font_size);
    ~Font();

    int GetSize();
    Vector2 ComputeStringBbox(const String &text, const float line_height);
    void FillTextBuffer(VertexBuffer *vbo, const String &text, const Vector4 &c, const float x, const float y, const float line_height);
    void Bind();
  private:
  };
};

#endif
