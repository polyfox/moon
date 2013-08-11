#ifndef MOON_FONT_H
#define MOON_FONT_H

#include "moon.hxx"
#include "shader.hxx"
#include "freetype-gl.h"
#include "vertex_buffer.hxx"

// FT_Get_Kerning( face, prev_index, glyph_index, FT_KERNING_UNFITTED, &kerning );

namespace Moon {
  class Font {
  public:
    Font(std::string name, int font_size);
    ~Font();

    void draw_text(float x, float y, const wchar_t *text); /* white text */
    void draw_text(float x, float y, const wchar_t *text, Color color);
  private:
    texture_font_t *font;
    texture_atlas_t *atlas;

    VertexBuffer buffer;
    void add_text(const wchar_t *text, Color c); /* temp */

    Shader shader;
  };
};

#endif