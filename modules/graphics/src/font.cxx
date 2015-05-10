#include <SOIL.h>
#include "moon/engine.hxx"
#include "moon/gl.h"
#include "moon/glm.h"
#include "moon/shader_loader.hxx"
#include "moon/font.hxx"
#include "moon/vector2.hxx"
#include "moon/vector4.hxx"

namespace Moon {
  Vector4 Font::DefaultColor = Vector4(1, 1, 1, 1);

  Font::Font(std::string filename, int font_size)
  : m_buffer(GL_DYNAMIC_DRAW)
  {
    m_shader = ShaderLoader::GetTextShader();
    m_atlas = texture_atlas_new(512, 512, 1);
    m_font = texture_font_new_from_file(m_atlas, font_size, filename.c_str());
    texture_font_load_glyphs(m_font, L" !\"#$%&'()*+,-./0123456789:;<=>?"
                                   L"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
                                   L"`abcdefghijklmnopqrstuvwxyz{|}~");
  }

  Font::~Font() {
    texture_font_delete(m_font);
    texture_atlas_delete(m_atlas);
  }

  void Font::DrawText(const float x, const float y, const float z,
                       const wchar_t *text) {
    DrawText(x, y, z, text, DefaultColor);
  }

  void Font::DrawText(const float x, const float y, const float z,
                      const wchar_t *text, const Vector4 &color) {
    RenderState render_ops;
    render_ops.color = color;
    DrawText(x, y, z, text, render_ops);
  }

  void Font::DrawText(const float x, const float y, const float z,
                      const wchar_t *text, const RenderState &render_ops) {
    // outline
    if (render_ops.outline > 0) {
      m_font->outline_type = 2;
      m_font->outline_thickness = render_ops.outline;
      AddText(text, render_ops.outline_color);
    }
    m_font->outline_type = 0;
    m_font->outline_thickness = 0;
    AddText(text, render_ops.color);
    m_shader->Use();
    glBindTexture(GL_TEXTURE_2D, m_atlas->id);
    glUniform1i(m_shader->GetUniform("tex"), /*GL_TEXTURE*/0);
    //model matrix
    glm::mat4 model_matrix = glm::rotate( // rotate it for 180 around the x-axis, because the text was upside down
      glm::translate(render_ops.transform, glm::vec3(x, y + m_font->ascender, z)), // move it to the correct position in the world
      glm::radians(180.0f),
      glm::vec3(1.0f, 0.0f, 0.0f)
    );
    // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
    glm::mat4 mvp_matrix = Shader::projection_matrix * Shader::view_matrix * model_matrix;
    glUniformMatrix4fv(m_shader->GetUniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));
    m_buffer.Render(GL_TRIANGLES);
    m_buffer.Clear();
  }

  void Font::AddText(const wchar_t *text, const Vector4 &c) {
    float cursor = 0; // position of the write cursor

    for(size_t i = 0; i < wcslen(text); ++i) {
      texture_glyph_t *glyph = texture_font_get_glyph(m_font, text[i]);
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
        Vertex vertices[4] = { {{x0,y0},  {s0,t0}, c},
                               {{x0,y1},  {s0,t1}, c},
                               {{x1,y1},  {s1,t1}, c},
                               {{x1,y0},  {s1,t0}, c} };
        m_buffer.PushBack(vertices, 4, indices, 6);
        cursor += glyph->advance_x;
      }
    }
  }

  Vector2 Font::ComputeStringBbox(const wchar_t *text) {
    Vector4 bbox;

    /* initialize string bbox to "empty" values */
    bbox.x = bbox.y =  32000;
    bbox.z = bbox.w = -32000;

    float cursor = 0; // position of the write cursor

    /* for each glyph image, compute its bounding box, */
    /* translate it, and grow the string bbox          */
    for (size_t i = 0; i < wcslen(text); ++i) {
      texture_glyph_t *glyph = texture_font_get_glyph(m_font, text[i]);

      if(glyph != NULL) {
        float kerning = 0;
        if(i > 0) {
          kerning = texture_glyph_get_kerning(glyph, text[i-1]);
        }
        cursor += kerning;
        float x0 = (cursor + glyph->offset_x);
        float y0 = glyph->offset_y;
        float x1  = x0 + glyph->width;
        float y1  = y0 - glyph->height;

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
    }

    /* return string bbox */
    return glm::vec2(bbox.z - bbox.x, bbox.w - bbox.y);
  }

  int Font::GetSize() {
    return m_font->size;
  }
}
