#ifndef MOON_FONT_H
#define MOON_FONT_H

#include "moon.hxx"
#include "shader.hxx"
#include "freetype-gl.h"
#include "vertex-buffer.h"

// FT_Get_Kerning( face, prev_index, glyph_index, FT_KERNING_UNFITTED, &kerning );

namespace Moon {
  class Font {
  public:
    Font(std::string name, int font_size);
    ~Font();

    void draw_text(float x, float y, wchar_t *text);
  private:
    texture_font_t *font;
    texture_atlas_t *atlas;

    vertex_buffer_t *buffer; /* de-wrap this later */
    void add_text(vertex_buffer_t *buffer, texture_font_t *font, wchar_t *text); /* temp */

    Shader shader;
  };
};

#endif