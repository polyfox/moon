#include "sprite.hxx"

namespace Moon {
  Sprite::Sprite(std::string filename)
  : VBO(GL_DYNAMIC_DRAW)
  {
    opacity = 1.0;

    angle = 0.0;
    ox = 0;
    oy = 0;
    color = moon_vec4(new glm::vec4(1.0, 1.0, 1.0, 1.0));
    tone = moon_vec4(new glm::vec4(0.0, 0.0, 0.0, 1.0));

    if (LEGACY_GL) {
      shader = Shader::load("resources/shaders/120/quad.vert", "resources/shaders/120/quad.frag");
    } else {
      shader = Shader::load("resources/shaders/330/quad.vert", "resources/shaders/330/quad.frag");
    }
    texture = Texture::load(filename);
    generate_buffers();
  };

  Sprite::~Sprite() {

  };

  bool Sprite::generate_buffers() {
    VBO.clear();
    // If the texture exists
    if(texture->id() != 0) {
      //Texture coordinates
      GLfloat s0 = 0.f;
      GLfloat s1 = 1.f;
      GLfloat t0 = 0.f;
      GLfloat t1 = 1.f;

      //Vertex coordinates
      GLfloat width = texture->width();
      GLfloat height = texture->height();

      //Handle clipping
      if(clip_rect) {
        //Texture coordinates
        s0 = (float)clip_rect->x / texture->width();
        s1 = (float)(clip_rect->x + clip_rect->w) / texture->width();
        t0 = (float)clip_rect->y / texture->height();
        t1 = (float)(clip_rect->y + clip_rect->h) / texture->height();

        //Vertex coordinates
        width = clip_rect->w;
        height = clip_rect->h;
      }

      vertex vertices[4] = {
        { {0.f,   0.f},    {s0, t0} },
        { {width, 0.f},    {s1, t0} },
        { {width, height}, {s1, t1} },
        { {0.f,   height}, {s0, t1} }
      };

      GLuint indices[4] = {0, 1, 3, 2}; // rendering indices

      VBO.push_back(vertices, 4, indices, 4);
      return true;
    };
    return false;
  };

  std::shared_ptr<Texture> Sprite::getTexture() {
    return texture;
  };

  // change Sprite's texture
  void Sprite::setTexture(std::shared_ptr<Texture> tex) {
    texture = std::move(tex); // passing by value already makes a copy
    generate_buffers();
  };

  std::shared_ptr<Rect> Sprite::getClip() {
    return clip_rect;
  };

  void Sprite::setClip(std::shared_ptr<Rect> clip) {
    clip_rect = std::move(clip); // passing by value already makes a copy
    generate_buffers();
  };

  void Sprite::render(const float &x, const float &y, const float &z) {
    shader->use();

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
    glUniformMatrix4fv(shader->get_uniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));

    glUniform1f(shader->get_uniform("opacity"), opacity);
    glUniform4fv(shader->get_uniform("color"), 1, glm::value_ptr(*color));
    glUniform4fv(shader->get_uniform("tone"), 1, glm::value_ptr(*tone));

    //Set texture ID
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    glUniform1i(shader->get_uniform("tex"), /*GL_TEXTURE*/0);

    VBO.render(GL_TRIANGLE_STRIP);
  };

};
