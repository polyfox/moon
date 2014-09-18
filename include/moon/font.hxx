#ifndef MOON_FONT_H
#define MOON_FONT_H

#include "moon/stdh.hxx"
#include "moon/shader.hxx"
#include "freetype-gl.h"
#include "vertex_buffer.hxx"

// FT_Get_Kerning( face, prev_index, glyph_index, FT_KERNING_UNFITTED, &kerning );

namespace Moon {
  class Font {
  public:
    Font(std::string name, int font_size);
    ~Font();

    void draw_text(const float &x, const float &y, const float &z, const wchar_t *text); /* white text */
    void draw_text(const float &x, const float &y, const float &z, const wchar_t *text, const glm::vec4 &color);
    void draw_text(const float &x, const float &y, const float &z, const wchar_t *text, const font_render_options &options);

    int size();
    glm::vec2 compute_string_bbox(const wchar_t *text);
  private:
    texture_font_t *font;
    texture_atlas_t *atlas;

    VertexBuffer buffer;
    void add_text(const wchar_t *text, const glm::vec4 &c);

    std::shared_ptr<Shader> shader;
  };
};

#endif
