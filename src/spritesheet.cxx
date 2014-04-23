#include "spritesheet.hxx"

namespace Moon {
  Spritesheet::Spritesheet(std::string filename, int tile_width, int tile_height)
  : VBO(GL_STATIC_DRAW)
  {
    shader = Shader::load("resources/shaders/quad.vert", "resources/shaders/quad.frag");
    texture = Texture::load(filename);

    this->tile_height = tile_height;
    this->tile_width = tile_width;
    total_sprites = 0;
    generate_buffers();
  };

  Spritesheet::~Spritesheet() {

  };

  bool Spritesheet::generate_buffers() {
    // If there is a texture loaded and clips to make vertex data from
    if(texture->id() != 0) {
      GLfloat tiles_per_row, tiles_per_column;

      tiles_per_row = texture->width() / tile_width;
      tiles_per_column = texture->height() / tile_height;

      total_sprites = tiles_per_row * tiles_per_column;

      for(int i = 0; i < total_sprites; ++i) {
        GLfloat ox = (float)(i % (int)tiles_per_row);
        GLfloat oy = (float)(i / (int)tiles_per_row);

        float s0 = (ox) / tiles_per_row;
        float s1 = (ox + 1.0) / tiles_per_row;
        float t0 = (oy) / tiles_per_column;
        float t1 = (oy + 1.0) / tiles_per_column;

        vertex vertices[4] = {
          { {0.f, 0.f},                {s0, t0} },
          { {tile_width, 0.f},         {s1, t0} },
          { {tile_width, tile_height}, {s1, t1} },
          { {0.f, tile_height},        {s0, t1} }
        };

        VBO.push_back_vertices(vertices, 4);
      }

      GLuint indices[4] = {0, 1, 3, 2};
      VBO.push_back_indices(indices, 4);

    } else {   //Error
      printf("No texture to render with!\n");
      return false;
    }

    return true;
  };

  void Spritesheet::render(const int &x, const int &y, const float &z,
                           const int &index, const render_options &render_ops) {

    // if you somehow managed to go out-of-bounds
    if ((index < 0) || (index >= (int)total_sprites)) return;
    if (texture->id() == 0) return;

    int offset = index*4;

    shader->use();

    //model matrix - move it to the correct position in the world
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
    glm::mat4 mvp_matrix = Shader::projection_matrix * Shader::view_matrix * model_matrix;
    glUniformMatrix4fv(shader->get_uniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));

    glUniform1f(shader->get_uniform("opacity"), render_ops.opacity);

    GLfloat rgbs[4] = { render_ops.tone.r, render_ops.tone.g, render_ops.tone.b, render_ops.tone.a };
    glUniform4fv(shader->get_uniform("tone"), 1, rgbs);

    GLfloat rgba[4] = { render_ops.color.r, render_ops.color.g, render_ops.color.b, render_ops.color.a };
    glUniform4fv(shader->get_uniform("color"), 1, rgba);

    //Set texture ID
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    glUniform1i(shader->get_uniform("tex"), /*GL_TEXTURE*/0);

    VBO.render_with_offset(GL_TRIANGLE_STRIP, offset);
  };

};
