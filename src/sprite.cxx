#include "sprite.hxx"

namespace Moon {
  Sprite::Sprite(std::string filename)
  : VBO(GL_DYNAMIC_DRAW)
  {
    x = 0;
    y = 0;
    z = 0.0;
    opacity = 1.0;
    tone = std::make_shared<Tone>(1.0, 1.0, 1.0);
    clip = false;

    shader = Shader::load("resources/shaders/quad.vert", "resources/shaders/quad.frag");
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
      /*if(clip != NULL) {
        //Texture coordinates
        s0 = clip->x / texture_width;
        s1 = (clip->x + clip->w) / texture_width;
        t0 = clip->y / texture_height;
        t1 = (clip->y + clip->h) / texture_height;

        //Vertex coordinates
        width = clip->w;
        height = clip->h;
      }*/

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

  // TODO: clipping
  void Sprite::render() {
    shader->use();

    //model matrix - move it to the correct position in the world
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
    glm::mat4 mvp_matrix = Shader::projection_matrix * Shader::view_matrix * model_matrix;
    glUniformMatrix4fv(shader->get_uniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp_matrix));

    glUniform1f(shader->get_uniform("opacity"), opacity);

    GLfloat hsl[3] = {tone->hue, tone->saturation, tone->lightness};
    glUniform3fv(shader->get_uniform("tone"), 1, hsl);

    //Set texture ID
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
    glUniform1i(shader->get_uniform("texture"), /*GL_TEXTURE*/0);

    VBO.render(GL_TRIANGLE_STRIP);
  };

};
