#include "moon/sprite.hxx"

namespace Moon {
  Sprite::Sprite()
  : m_VBO(GL_DYNAMIC_DRAW)
  {
    opacity = 1.0;

    angle = 0.0;
    ox = 0;
    oy = 0;
    color = moon_vec4(new glm::vec4(1.0, 1.0, 1.0, 1.0));
    tone = moon_vec4(new glm::vec4(0.0, 0.0, 0.0, 1.0));

    if (LEGACY_GL) {
      m_shader = Shader::load("resources/shaders/120/quad.vert", "resources/shaders/120/quad.frag");
    } else {
      m_shader = Shader::load("resources/shaders/330/quad.vert", "resources/shaders/330/quad.frag");
    }
  };

  Sprite::~Sprite() {

  };

  void Sprite::load_texture(moon_texture texture) {
    m_texture = texture;
    generate_buffers();
  }

  void Sprite::load_file(std::string filename) {
    load_texture(Texture::load(filename));
  }

  bool Sprite::generate_buffers() {
    m_VBO.clear();
    // If the texture exists
    if (m_texture->id() != 0) {
      //Texture coordinates
      GLfloat s0 = 0.f;
      GLfloat s1 = 1.f;
      GLfloat t0 = 0.f;
      GLfloat t1 = 1.f;

      //Vertex coordinates
      GLfloat width = m_texture->width();
      GLfloat height = m_texture->height();

      //Handle clipping
      if(m_clip_rect) {
        //Texture coordinates
        s0 = (float)m_clip_rect->x / m_texture->width();
        s1 = (float)(m_clip_rect->x + m_clip_rect->w) / m_texture->width();
        t0 = (float)m_clip_rect->y / m_texture->height();
        t1 = (float)(m_clip_rect->y + m_clip_rect->h) / m_texture->height();

        //Vertex coordinates
        width = m_clip_rect->w;
        height = m_clip_rect->h;
      }

      vertex vertices[4] = {
        { {0.f,   0.f},    {s0, t0} },
        { {width, 0.f},    {s1, t0} },
        { {width, height}, {s1, t1} },
        { {0.f,   height}, {s0, t1} }
      };

      GLuint indices[4] = {0, 1, 3, 2}; // rendering indices

      m_VBO.push_back(vertices, 4, indices, 4);
      return true;
    };
    return false;
  };

  moon_texture Sprite::getTexture() {
    return m_texture;
  };

  // change Sprite's texture
  void Sprite::setTexture(moon_texture tex) {
    m_texture = std::move(tex); // passing by value already makes a copy
    generate_buffers();
  };

  moon_rect Sprite::getClip() {
    return m_clip_rect;
  };

  void Sprite::setClip(moon_rect clip) {
    m_clip_rect = std::move(clip); // passing by value already makes a copy
    generate_buffers();
  };

  void Sprite::render(const float &x, const float &y, const float &z) {
    m_shader->use();

    // rotation matrix - rotate the model around specified origin
    // really ugly, we translate the rotation origin to 0,0, rotate,
    // then translate back to original position
    glm::mat4 rotation_matrix = glm::translate(glm::rotate(
      glm::translate(glm::mat4(1.0f), glm::vec3(ox, oy, 0)),
      glm::radians(angle),
      glm::vec3(0, 0, 1)
    ), glm::vec3(-ox, -oy, 0));

    // model matrix - move it to the correct position in the world
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
    glm::mat4 mvp_matrix = Shader::projection_matrix * Shader::view_matrix * model_matrix * rotation_matrix;
    glUniformMatrix4fv(m_shader->get_uniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));

    glUniform1f(m_shader->get_uniform("opacity"), opacity);
    glUniform4fv(m_shader->get_uniform("color"), 1, glm::value_ptr(*color));
    glUniform4fv(m_shader->get_uniform("tone"), 1, glm::value_ptr(*tone));

    //Set texture ID
    glActiveTexture(GL_TEXTURE0);
    m_texture->bind();
    glUniform1i(m_shader->get_uniform("tex"), /*GL_TEXTURE*/0);

    m_VBO.render(GL_TRIANGLE_STRIP);
  };

};
