#include "texture.hxx"

namespace Moon {
  Texture::Texture(std::string filename)
  : Cache(filename)
  {
    unsigned char* pixels;
    int channels;
    float border_color[4] = { 0.0, 0.0, 0.0, 0.0 };

    pixels = SOIL_load_image(filename.c_str(), &texture_width, &texture_height, &channels, SOIL_LOAD_AUTO);
    gl_texture_id = SOIL_create_OGL_texture(pixels, texture_width, texture_height, channels, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA);

    glBindTexture(GL_TEXTURE_2D, gl_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    glBindTexture(GL_TEXTURE_2D, 0);
  };

  Texture::~Texture() {
    //Delete texture
    if(gl_texture_id != 0) {
      glDeleteTextures(1, &gl_texture_id);
    }
  };

  GLint Texture::width() {
    return texture_width;
  };

  GLint Texture::height() {
    return texture_height;
  };

  GLuint Texture::id() {
    return gl_texture_id;
  };

  void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, gl_texture_id);
  };

}
