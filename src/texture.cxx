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

  GLuint Texture::width() {
    return texture_width;
  };

  GLuint Texture::height() {
    return texture_height;
  };

  GLuint Texture::id() {
    return texture_id;
  };

  void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, texture_id);
  };

}