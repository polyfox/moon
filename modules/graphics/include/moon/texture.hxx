#ifndef MOON_TEXTURE_H
#define MOON_TEXTURE_H

#include <memory>
#include <SOIL.h>
#include "moon/intern.h"
#include "moon/gl.h"
#include "moon/shader.hxx"
#include "moon/vertex_buffer.hxx"

namespace Moon {
  class Texture {
  public:
    Texture(std::string filename);
    Texture(int width, int height);
    ~Texture();
    void Bind();
    void Attach();
    GLint GetWidth();
    GLint GetHeight();
    GLuint GetID();
  private:
    GLint m_width;
    GLint m_height;
    GLuint m_gl_texture_id;
  };
};

#endif
