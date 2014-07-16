#include "font.hxx"
#include "SOIL.h"

namespace Moon {
  Font::Font(std::string filename, int font_size)
  : buffer(GL_DYNAMIC_DRAW)
  {
    if (!GLEW_VERSION_3_3) {
      shader = Shader::load("resources/shaders/120/text.vert", "resources/shaders/120/text.frag");
    } else {
      shader = Shader::load("resources/shaders/330/text.vert", "resources/shaders/330/text.frag");
    }
    atlas = texture_atlas_new(512, 512, 1);
    font = texture_font_new(atlas, filename.c_str(), font_size);

    texture_font_load_glyphs(font, L" !\"#$%&'()*+,-./0123456789:;<=>?"
                                   L"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
                                   L"`abcdefghijklmnopqrstuvwxyz{|}~");
  }

  Font::~Font() {
    texture_font_delete(font);
    texture_atlas_delete(atlas);
  }

  void Font::draw_text(const float &x, const float &y, const float &z,
                       const wchar_t *text) {
    glm::vec4 color(1.0, 1.0, 1.0, 1.0);
    draw_text(x, y, z, text, color);
  }

  void Font::draw_text(const float &x, const float &y, const float &z,
                       const wchar_t *text, const glm::vec4 &color) {
    font_render_options render_ops;
    render_ops.color = color;
    draw_text(x, y, z, text, render_ops);
  }

  void Font::draw_text(const float &x, const float &y, const float &z,
                       const wchar_t *text, const font_render_options &render_ops) {
    // outline
    if (render_ops.outline > 0) {
      font->outline_type = 2;
      font->outline_thickness = render_ops.outline;
      add_text(text, render_ops.outline_color);
    }

    font->outline_type = 0;
    font->outline_thickness = 0;
    add_text(text, render_ops.color);

    shader->use();

    glBindTexture(GL_TEXTURE_2D, atlas->id);

    glUniform1i(shader->get_uniform("tex"), /*GL_TEXTURE*/0);

    //model matrix
    glm::mat4 model_matrix = glm::rotate( // rotate it for 180 around the x-axis, because the text was upside down
      glm::translate(render_ops.transform, glm::vec3(x, y + font->ascender, z)), // move it to the correct position in the world
      glm::radians(180.0f),
      glm::vec3(1.0f, 0.0f, 0.0f)
    );
    // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
    glm::mat4 mvp_matrix = Shader::projection_matrix * Shader::view_matrix * model_matrix;
    glUniformMatrix4fv(shader->get_uniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));

    buffer.render(GL_TRIANGLES);
    buffer.clear();
  }

  void Font::add_text(const wchar_t *text, const glm::vec4 &c) {
    float cursor = 0; // position of the write cursor

    for(size_t i = 0; i < wcslen(text); ++i) {
      texture_glyph_t *glyph = texture_font_get_glyph(font, text[i]);
      if(glyph != NULL) {
        float kerning = 0;
        if(i > 0) {
          kerning = texture_glyph_get_kerning(glyph, text[i-1]);
        }
        cursor += kerning;
        float x0  = cursor + glyph->offset_x;
        float y0  = glyph->offset_y;
        float x1  = x0 + glyph->width;
        float y1  = y0 - glyph->height;

        float s0 = glyph->s0;
        float t0 = glyph->t0;
        float s1 = glyph->s1;
        float t1 = glyph->t1;

        GLuint indices[6] = {0,1,2, 0,2,3};
        vertex vertices[4] = { {{x0,y0},  {s0,t0}, c},
                               {{x0,y1},  {s0,t1}, c},
                               {{x1,y1},  {s1,t1}, c},
                               {{x1,y0},  {s1,t0}, c} };
        buffer.push_back(vertices, 4, indices, 6);
        cursor += glyph->advance_x;
      }
    }
  }

  glm::vec2 Font::compute_string_bbox(const wchar_t *text) {
    bbox_t bbox;

    /* initialize string bbox to "empty" values */
    bbox.xMin = bbox.yMin =  32000;
    bbox.xMax = bbox.yMax = -32000;

    float cursor = 0; // position of the write cursor

    /* for each glyph image, compute its bounding box, */
    /* translate it, and grow the string bbox          */
    for(size_t i = 0; i < wcslen(text); ++i) {
      texture_glyph_t *glyph = texture_font_get_glyph(font, text[i]);

      if(glyph != NULL) {
        float kerning = 0;
        if(i > 0) {
          kerning = texture_glyph_get_kerning(glyph, text[i-1]);
        }
        cursor += kerning;
        float x0 = (cursor + glyph->offset_x);
        float y0 = glyph->offset_y;

        if (glyph->bbox.xMin + x0 < bbox.xMin)
          bbox.xMin = glyph->bbox.xMin + x0;

        if (glyph->bbox.yMin + y0 < bbox.yMin)
          bbox.yMin = glyph->bbox.yMin + y0;

        if (glyph->bbox.xMax + x0 > bbox.xMax)
          bbox.xMax = glyph->bbox.xMax + x0;

        if (glyph->bbox.yMax + y0 > bbox.yMax)
          bbox.yMax = glyph->bbox.yMax + y0;

        cursor += glyph->advance_x;
      }
    }

    /* check that we really grew the string bbox */
    if (bbox.xMin > bbox.xMax) {
      bbox.xMin = 0;
      bbox.yMin = 0;
      bbox.xMax = 0;
      bbox.yMax = 0;
    }

    /* return string bbox */
    int width  = bbox.xMax - bbox.xMin;
    int height = bbox.yMax - bbox.yMin;

    return glm::vec2(width, height);
  }

  int Font::size() {
    return font->size;
  }
  /*GlyphMap::GlyphMap() {

  };*/
}