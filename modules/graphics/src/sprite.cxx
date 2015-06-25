#include "moon/engine.hxx"
#include "moon/sprite.hxx"
#include "moon/shader.hxx"
#include "moon/texture.hxx"
#include "moon/vector4.hxx"

namespace Moon {
  Sprite::Sprite() : m_vbo(GL_DYNAMIC_DRAW)
  {
    shader = NULL;
    use_clip = false;
    opacity = 1.0;
    angle = 0.0;
    origin = Vector2(0, 0);
    color = Vector4(1.0, 1.0, 1.0, 1.0);
    tone = Vector4(0.0, 0.0, 0.0, 1.0);
    m_texture = NULL;
  };

  void Sprite::Render(const float x, const float y, const float z) {
    if (!m_texture) return;
    if (!shader) return;

    shader->Use();
    // rotation matrix - rotate the model around specified origin
    // really ugly, we translate the rotation origin to 0,0, rotate,
    // then translate back to original position
    glm::mat4 rotation_matrix = glm::translate(glm::rotate(
      glm::translate(glm::mat4(1.0f), glm::vec3(origin.x, origin.y, 0)),
      glm::radians(angle),
      glm::vec3(0, 0, 1)
    ), glm::vec3(-origin.x, -origin.y, 0));
    // model matrix - move it to the correct position in the world
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
    glm::mat4 mvp_matrix = Shader::projection_matrix * Shader::view_matrix * model_matrix * rotation_matrix;
    glUniformMatrix4fv(shader->GetUniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));
    glUniform1f(shader->GetUniform("opacity"), opacity);
    glUniform4fv(shader->GetUniform("color"), 1, glm::value_ptr(color));
    glUniform4fv(shader->GetUniform("tone"), 1, glm::value_ptr(tone));
    //Set texture ID
    glActiveTexture(GL_TEXTURE0);
    m_texture->Bind();
    glUniform1i(shader->GetUniform("tex"), /*GL_TEXTURE*/0);
    m_vbo.Render(GL_TRIANGLE_STRIP);
  };
};
