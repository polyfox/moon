#include "spritesheet.hxx"

namespace Moon {
  Spritesheet::Spritesheet()
  : VBO(GL_STATIC_DRAW)
  {
    if (LEGACY_GL) {
      m_shader = Shader::load("resources/shaders/120/quad.vert", "resources/shaders/120/quad.frag");
    } else {
      m_shader = Shader::load("resources/shaders/330/quad.vert", "resources/shaders/330/quad.frag");
    }
  }

  Spritesheet::~Spritesheet() {

  };

  void Spritesheet::load_texture(moon_texture texture, int tile_width, int tile_height) {
    m_texture = texture;

    this->tile_height = tile_height;
    this->tile_width = tile_width;

    total_sprites = 0;

    generate_buffers();
  }

  void Spritesheet::load_file(std::string filename, int tile_width, int tile_height) {
    load_texture(Texture::load(filename), tile_width, tile_height);
  }

  bool Spritesheet::generate_buffers() {
    // If there is a texture loaded and clips to make vertex data from
    if(m_texture->id() != 0) {
      GLfloat tiles_per_row, tiles_per_column;

      tiles_per_row = m_texture->width() / tile_width;
      tiles_per_column = m_texture->height() / tile_height;

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

  void Spritesheet::render(const float &x, const float &y, const float &z,
                           const int &index, const ss_render_options &render_ops) {

    // if you somehow managed to go out-of-bounds
    if ((index < 0) || (index >= (int)total_sprites)) return;
    if (m_texture->id() == 0) return;

    int offset = index*4;

    m_shader->use();

    //model matrix - move it to the correct position in the world
    glm::mat4 model_matrix = glm::translate(render_ops.transform, glm::vec3(x, y, z));
    glm::mat4 rotation_matrix = glm::translate(glm::rotate(
      glm::translate(glm::mat4(1.0f), glm::vec3(render_ops.ox, render_ops.oy, 0)),
      glm::radians(render_ops.angle),
      glm::vec3(0, 0, 1)
    ), glm::vec3(-render_ops.ox, -render_ops.oy, 0));

    // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
    glm::mat4 mvp_matrix = Shader::projection_matrix * Shader::view_matrix * model_matrix * rotation_matrix;
    glUniformMatrix4fv(m_shader->get_uniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));

    glUniform1f(m_shader->get_uniform("opacity"), render_ops.opacity);
    glUniform4fv(m_shader->get_uniform("tone"), 1, glm::value_ptr(render_ops.tone));
    glUniform4fv(m_shader->get_uniform("color"), 1, glm::value_ptr(render_ops.color));

    //Set texture ID
    glActiveTexture(GL_TEXTURE0);
    m_texture->bind();
    glUniform1i(m_shader->get_uniform("tex"), /*GL_TEXTURE*/0);

    VBO.render_with_offset(GL_TRIANGLE_STRIP, offset);
  };
};
