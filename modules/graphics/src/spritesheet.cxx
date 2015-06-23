#include "moon/engine.hxx"
#include "moon/spritesheet.hxx"
#include "moon/texture.hxx"
#include "moon/vector4.hxx"

namespace Moon {
  Spritesheet::Spritesheet() : m_vbo(GL_STATIC_DRAW)
  {
    shader = NULL;
  }

  Spritesheet::~Spritesheet() {

  };

  void Spritesheet::LoadTexture(Texture *texture, int tile_width, int tile_height) {
    m_texture = texture;

    this->tile_height = tile_height;
    this->tile_width = tile_width;

    total_sprites = 0;

    GenerateBuffers();
  }

  bool Spritesheet::GenerateBuffers() {
    // If there is a texture loaded and clips to make vertex data from
    if (m_texture->GetID() != 0) {
      GLfloat tiles_per_row, tiles_per_column;

      tiles_per_row = m_texture->GetWidth() / tile_width;
      tiles_per_column = m_texture->GetHeight() / tile_height;

      total_sprites = tiles_per_row * tiles_per_column;

      for(int i = 0; i < total_sprites; ++i) {
        GLfloat ox = (float)(i % (int)tiles_per_row);
        GLfloat oy = (float)(i / (int)tiles_per_row);

        float s0 = (ox) / tiles_per_row;
        float s1 = (ox + 1.0) / tiles_per_row;
        float t0 = (oy) / tiles_per_column;
        float t1 = (oy + 1.0) / tiles_per_column;

        Vertex vertices[4] = {
          { {0.f, 0.f},                {s0, t0}, Vector4(0, 0, 0, 0) },
          { {tile_width, 0.f},         {s1, t0}, Vector4(0, 0, 0, 0) },
          { {tile_width, tile_height}, {s1, t1}, Vector4(0, 0, 0, 0) },
          { {0.f, tile_height},        {s0, t1}, Vector4(0, 0, 0, 0) }
        };

        m_vbo.PushBackVertices(vertices, 4);
      }

      GLuint indices[4] = {0, 1, 3, 2};
      m_vbo.PushBackIndices(indices, 4);

    } else {   //Error
      printf("No texture to render with!\n");
      return false;
    }

    return true;
  };

  void Spritesheet::Render(const float x, const float y, const float z,
                           const int index, const Spritesheet::RenderState &render_ops) {

    // if you somehow managed to go out-of-bounds
    if ((index < 0) || (index >= (int)total_sprites)) return;
    if (m_texture->GetID() == 0) return;
    if (!shader) return;

    int offset = index * 4;

    shader->Use();

    //model matrix - move it to the correct position in the world
    Vector2 origin = render_ops.origin;
    glm::mat4 model_matrix = glm::translate(render_ops.transform, glm::vec3(x, y, z));
    glm::mat4 rotation_matrix = glm::translate(glm::rotate(
      glm::translate(glm::mat4(1.0f), glm::vec3(origin.x, origin.y, 0)),
      glm::radians(render_ops.angle),
      glm::vec3(0, 0, 1)
    ), glm::vec3(-origin.x, -origin.y, 0));

    // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
    glm::mat4 mvp_matrix = Shader::projection_matrix * Shader::view_matrix * model_matrix * rotation_matrix;
    glUniformMatrix4fv(shader->GetUniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));

    glUniform1f(shader->GetUniform("opacity"), render_ops.opacity);
    glUniform4fv(shader->GetUniform("tone"), 1, glm::value_ptr(render_ops.tone));
    glUniform4fv(shader->GetUniform("color"), 1, glm::value_ptr(render_ops.color));

    //Set texture ID
    glActiveTexture(GL_TEXTURE0);
    m_texture->Bind();
    glUniform1i(shader->GetUniform("tex"), /*GL_TEXTURE*/0);

    m_vbo.RenderWithOffset(GL_TRIANGLE_STRIP, offset);
  };
};
