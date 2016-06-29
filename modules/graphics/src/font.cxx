#include <SOIL.h>
#include "moon/gl.h"
#include "moon/glm.h"
#include "moon/font.hxx"
#include "moon/string.hxx"
#include "moon/vector2.hxx"
#include "moon/vector4.hxx"

#define isNewline(_char_) ((_char_) == '\n' || (_char_) == '\r')

namespace Moon {
  Font::Font(const std::string &filename, const int font_size)
  {
    atlas = texture_atlas_new(1024, 1024, 1);
    font = texture_font_new_from_file(atlas, font_size, filename.c_str());
    texture_font_load_glyphs(font, " !\"#$%&'()*+,-./0123456789:;<=>?"
                                   "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
                                   "`abcdefghijklmnopqrstuvwxyz{|}~");
    texture_atlas_upload(atlas);
  }

  Font::~Font() {
    texture_font_delete(font);
    texture_atlas_delete(atlas);
  }

  void Font::FillTextBuffer(VertexBuffer *vbo, const String &text, const Vector4 &c,
   const float x, const float y, const float line_height) {
    const float line_space = line_height * font->size;
    float cursor = 0; // position of the write cursor
    float line = 0;
    for(size_t i = 0; i < text.length(); ++i) {
      if (isNewline(text[i])) {
        cursor = 0;
        line += line_space;
        continue;
      }

      texture_glyph_t *glyph = texture_font_get_glyph(font, text.get_cptr(i));
      if (glyph != NULL) {
        float kerning = 0;
        if (i > 0) {
          kerning = texture_glyph_get_kerning(glyph, text.get_cptr(i - 1));
        }
        cursor += kerning;
        float x0 = x + cursor + glyph->offset_x;
        float y0 = y + line - glyph->offset_y;
        float x1 = x0 + glyph->width;
        float y1 = y0 + glyph->height;

        float s0 = glyph->s0;
        float t0 = glyph->t0;
        float s1 = glyph->s1;
        float t1 = glyph->t1;

        GLuint indices[6] = {0,1,2, 0,2,3};
        Vertex vertices[4] = { {{x0,y0},  {s0,t0}, c},
                               {{x0,y1},  {s0,t1}, c},
                               {{x1,y1},  {s1,t1}, c},
                               {{x1,y0},  {s1,t0}, c} };
        vbo->PushBack(vertices, 4, indices, 6);
        cursor += glyph->advance_x;
      }
    }
    // upload to the buffer, we might have loaded more glyphs
    texture_atlas_upload(atlas);
  }

  Vector2 Font::ComputeStringBbox(const String &text, const float line_height) {
    Vector4 bbox;
    const float line_space = line_height * font->size;
    float cursor = 0; // position of the write cursor
    float line = 0;

    /* initialize string bbox to "empty" values */
    bbox.x = bbox.y =  32000;
    bbox.z = bbox.w = -32000;

    /* for each glyph image, compute its bounding box, */
    /* translate it, and grow the string bbox          */
    for (size_t i = 0; i < text.length(); ++i) {
      if (isNewline(text[i])) {
        if (cursor > bbox.z) bbox.z = cursor;
        cursor = 0;
        line += line_space;
        continue;
      }

      texture_glyph_t *glyph = texture_font_get_glyph(font, text.get_cptr(i));

      if (glyph != NULL) {
        float kerning = 0;
        if(i > 0) {
          kerning = texture_glyph_get_kerning(glyph, text.get_cptr(i - 1));
        }
        cursor += kerning;
        float x0 = cursor + glyph->offset_x;
        float y0 = line - glyph->offset_y;
        float x1 = x0 + glyph->width;
        float y1 = y0 + glyph->height;

        if (x0 < bbox.x)
          bbox.x = x0;

        if (y0 < bbox.y)
          bbox.y = y0;

        if (x1 > bbox.z)
          bbox.z = x1;

        if (y1 > bbox.w)
          bbox.w = y1;

        cursor += glyph->advance_x;
      }
    }

    /* check that we really grew the string bbox */
    if (bbox.x > bbox.z) {
      bbox.x = 0;
      bbox.y = 0;
      bbox.z = 0;
      bbox.w = 0;
    } else {
      // adjusts bbox.z position to compensate for the last character
      // glyph->advance_x
      if (cursor > bbox.z) bbox.z = cursor;
    }

    /* return string bbox */
    return glm::vec2(bbox.z - bbox.x, bbox.w - bbox.y);
  }

  int Font::GetSize() {
    return font->size;
  }

  void Font::Bind() {
    glBindTexture(GL_TEXTURE_2D, atlas->id);
  }
}
