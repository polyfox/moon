#include "texture.hxx"
#include <SOIL.h>

namespace Moon {
  Texture::Texture(std::string filename)
  : Cache(filename)
  {
    shader = Shader::load("resources/shaders/quad.vert", "resources/shaders/quad.frag");

    unsigned char* pixels;
    int channels;

    pixels = SOIL_load_image(filename.c_str(), &texture_width, &texture_height, &channels, SOIL_LOAD_RGBA);
    texture_id = SOIL_create_OGL_texture(pixels, texture_width, texture_height, channels, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
  };

  Texture::~Texture() {
    //Delete texture
    if(texture_id != 0) {
      glDeleteTextures(1, &texture_id);
    }
  };

  std::shared_ptr<Texture> Texture::load(std::string filename) {
    auto ptr = std::move(get(filename)); // move it into the new pointer, so we don't copy construct

    if (ptr) return ptr; // cache hit

    ptr = std::move(std::shared_ptr<Texture>(new Texture(filename)));
    // ptr = std::make_shared<Texture>(filename); is supposedly faster,
    // we just need to get around the private constructor issue
    Cache::_cache[filename] = ptr;
    return ptr;
  };

  GLuint Texture::width() {
    return texture_width;
  };

  GLuint Texture::height() {
    return texture_height;
  };

  GLuint Texture::id() {
    return texture_id;
  };

  void Texture::render_with_offset(const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &opacity, Tone *tone, VertexBuffer &vbo, const int &offset) {
    if(texture_id != 0) {
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
      glBindTexture(GL_TEXTURE_2D, texture_id);
      glUniform1i(shader->get_uniform("texture"), /*GL_TEXTURE*/0);

      vbo.render_with_offset(GL_TRIANGLE_STRIP, offset);
    };
  };

  void Texture::render(const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &opacity, Tone *tone, VertexBuffer &vbo) {
    if(texture_id != 0) {
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
      glBindTexture(GL_TEXTURE_2D, texture_id);
      glUniform1i(shader->get_uniform("texture"), /*GL_TEXTURE*/0);

      vbo.render(GL_TRIANGLE_STRIP);
    };
  };

}