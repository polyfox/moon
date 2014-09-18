#ifndef MOON_TEXTURE_H
#define MOON_TEXTURE_H

#include "moon/stdh.hxx"
//#include "graphics.hxx"
#include "moon/cache.hxx"
#include "moon/shader.hxx"
#include <memory>
#include <SOIL.h>

#include "moon/vertex_buffer.hxx"

namespace Moon {

  class Texture: public Cache<Texture> {
  public:
    ~Texture();

    void bind();

    GLint width();
    GLint height();
    GLuint id();
  private:
    Texture(std::string filename);

    GLint texture_width;
    GLint texture_height;
    GLuint gl_texture_id;
  friend class Cache<Texture>;
  };

};

#endif
