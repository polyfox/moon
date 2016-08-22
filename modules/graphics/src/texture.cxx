#include <SOIL.h>
#include "moon/texture.hxx"
#include "moon/gl.h"

namespace Moon {
  Texture::Texture(std::string filename)
  {
    unsigned char* pixels;
    int channels;
    float border_color[4] = { 0.0, 0.0, 0.0, 0.0 };

    pixels = SOIL_load_image(filename.c_str(), &m_width, &m_height, &channels, SOIL_LOAD_AUTO);
    m_gl_texture_id = SOIL_create_OGL_texture(pixels, m_width, m_height, channels, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA);
    SOIL_free_image_data(pixels);

    glBindTexture(GL_TEXTURE_2D, m_gl_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    glBindTexture(GL_TEXTURE_2D, 0);
  };

  Texture::Texture(int width, int height)
  {
    float border_color[4] = { 0.0, 0.0, 0.0, 0.0 };
    // Generate empty (RGB) texture
    glGenTextures(1, &m_gl_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_gl_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    glBindTexture(GL_TEXTURE_2D, 0);
  };

  Texture::~Texture() {
    //Delete texture
    if (m_gl_texture_id != 0) {
      glDeleteTextures(1, &m_gl_texture_id);
    }
  };

  GLint Texture::GetWidth() {
    return m_width;
  };

  GLint Texture::GetHeight() {
    return m_height;
  };

  GLuint Texture::GetID() {
    return m_gl_texture_id;
  };

  void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, m_gl_texture_id);
  };

  void Texture::Attach() {
    // Attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gl_texture_id, 0);
  };
}
