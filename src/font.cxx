#include "font.hxx"
#include "SOIL.h"

namespace Moon {
  Font::Font(std::string filename, int font_size) 
  : shader("resources/shaders/text.vert", "resources/shaders/text.frag"),
    buffer(GL_DYNAMIC_DRAW)
  {
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

  void Font::draw_text(float x, float y, const wchar_t *text) {
    Color color = {1.0,1.0,1.0,1.0};
    draw_text(x, y, text, color);
  }

  void Font::draw_text(float x, float y, const wchar_t *text, Color color) {

    // outline
    font->outline_type = 2;
    font->outline_thickness = 1;
    add_text(text, {0.0, 0.0, 0.0, 0.9});

    font->outline_type = 0;
    font->outline_thickness = 0;
    add_text(text, color);

    shader.use();

    glBindTexture(GL_TEXTURE_2D, atlas->id);

    glUniform1i(shader.get_uniform("texture"), /*GL_TEXTURE*/0);

    //model matrix 
    glm::mat4 model_matrix = glm::rotate( // rotate it for 180 around the x-axis, because the text was upside down
      glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0)), // move it to the correct position in the world
      180.0f, 
      glm::vec3(1.0f, 0.0f, 0.0f)
    );
    glUniformMatrix4fv(shader.get_uniform("model_matrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

    //projection matrix
    glUniformMatrix4fv(shader.get_uniform("projection_matrix"), 1, GL_FALSE, glm::value_ptr(Shader::projection_matrix));

    buffer.render(GL_TRIANGLES, shader.get_attribute("vertex_pos"), shader.get_attribute("tex_coord"), shader.get_attribute("color"));
    buffer.clear();
  }

  void Font::add_text(const wchar_t *text, Color c) {
    float cursor = 0; // position of the write cursor

    for(size_t i = 0; i < wcslen(text); ++i) {
      texture_glyph_t *glyph = texture_font_get_glyph(font, text[i]);
      if(glyph != NULL) {
        int kerning = 0;
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
        vertex vertices[4] = { {x0,y0,  s0,t0, c},
                               {x0,y1,  s0,t1, c},
                               {x1,y1,  s1,t1, c},
                               {x1,y0,  s1,t0, c} };
        buffer.push_back(vertices, 4, indices, 6);
        cursor += glyph->advance_x;
      }
    }
    buffer.upload();
  }

  /*GlyphMap::GlyphMap() {

  };*/
}