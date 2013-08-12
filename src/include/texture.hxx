#ifndef MOON_TEXTURE_H
#define MOON_TEXTURE_H

#include "moon.hxx"
#include "graphics.hxx"
#include "cache.hxx"
#include "shader.hxx"
#include <memory>
#include "tone.hxx"

#include "vertex_buffer.hxx"

namespace Moon {

  class Texture: public Cache<Texture> {
  public:
    ~Texture();

    void render_with_offset(const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &opacity, Tone *tone, VertexBuffer &vbo, const int &offset);

    void bind();

    GLuint width();
    GLuint height();
    GLuint id();
  private:
    Texture(std::string filename);

    std::shared_ptr<Shader> shader;

    GLuint texture_id;
    GLint texture_width;
    GLint texture_height;

  friend class Cache<Texture>;
  };

};

#endif