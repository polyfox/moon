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

namespace Moon {
  struct font_render_options {
    float outline;
    Moon::Vector4 color;
    Moon::Vector4 outline_color;
    Moon::Transform transform;
    font_render_options() :
      outline(0.0),
      color(1.0, 1.0, 1.0, 1.0),
      outline_color(0.0, 0.0, 0.0, 0.9)
    {
    };
  };

  class Font {
  public:
    Font(std::string name, int font_size);
    ~Font();

    void draw_text(const float &x, const float &y, const float &z, const wchar_t *text); /* white text */
    void draw_text(const float &x, const float &y, const float &z, const wchar_t *text, const Moon::Vector4 &color);
    void draw_text(const float &x, const float &y, const float &z, const wchar_t *text, const font_render_options &options);

    int size();
    Moon::Vector2 compute_string_bbox(const wchar_t *text);
  private:
    texture_font_t *m_font;
    texture_atlas_t *m_atlas;
    VertexBuffer m_buffer;
    Shader *m_shader;
    void add_text(const wchar_t *text, const Moon::Vector4 &c);
  };
};

#endif
