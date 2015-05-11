#ifndef MOON_FONT_H
#define MOON_FONT_H

#include <freetype-gl.h>
#include "moon/glm.h"
#include "moon/intern.h"
#include "moon/shader.hxx"
#include "moon/vertex_buffer.hxx"
#include "moon/vector2.hxx"
#include "moon/vector4.hxx"
#include "moon/transform.hxx"
#include "moon/string.hxx"

namespace Moon {
  class Font {
  public:
    struct RenderState {
      GLfloat outline;
      Vector4 color;
      Vector4 outline_color;
      Transform transform;

      RenderState() :
        outline(0.0),
        color(1.0, 1.0, 1.0, 1.0),
        outline_color(0.0, 0.0, 0.0, 0.9)
      {
      };
    };

    static Vector4 DefaultColor;
    Font(std::string name, int font_size);
    ~Font();

    void DrawText(const float x, const float y, const float z, const Moon::String &text); /* white text */
    void DrawText(const float x, const float y, const float z, const Moon::String &text, const Vector4 &color);
    void DrawText(const float x, const float y, const float z, const Moon::String &text, const RenderState &options);
    int GetSize();
    Vector2 ComputeStringBbox(const Moon::String &text);
  private:
    texture_font_t *m_font;
    texture_atlas_t *m_atlas;
    VertexBuffer m_buffer;
    Shader *m_shader;
    void AddText(const Moon::String &text, const Vector4 &c);
  };
};

#endif
