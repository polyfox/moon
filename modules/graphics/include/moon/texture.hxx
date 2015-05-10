#ifndef MOON_TEXTURE_H
#define MOON_TEXTURE_H

#include <memory>
#include <SOIL.h>
#include "moon/intern.h"
#include "moon/gl.h"
#include "moon/cache.hxx"
#include "moon/shader.hxx"
#include "moon/vertex_buffer.hxx"

namespace Moon {
  class Texture: public Cache<Texture> {
  public:
    Texture(std::string filename);
    ~Texture();
    void Bind();
    GLint GetWidth();
    GLint GetHeight();
    GLuint GetID();
  private:
    GLint m_width;
    GLint m_height;
    GLuint m_gl_texture_id;
  friend class Cache<Texture>;
  };
};

#endif
