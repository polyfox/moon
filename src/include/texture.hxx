#ifndef MOON_TEXTURE_H
#define MOON_TEXTURE_H

#include "moon.hxx"
#include "graphics.hxx"
#include "cache.hxx"
#include "shader.hxx"
#include <memory>
#include "tone.hxx"
#include <SOIL.h>

#include "vertex_buffer.hxx"

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

    std::shared_ptr<Shader> shader;

    GLint texture_width;
    GLint texture_height;
    GLuint texture_id;

  friend class Cache<Texture>;
  };

};

#endif