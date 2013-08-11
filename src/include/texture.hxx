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

    static std::shared_ptr<Texture> load(std::string filename);
    void render_with_offset(const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &opacity, Tone *tone, VertexBuffer &vbo, const int &offset);

    void render(const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &opacity, Tone *tone, VertexBuffer &vboID);

    GLuint width();
    GLuint height();
    GLuint id();
  private:
    Texture(std::string filename);

    Shader shader;

    GLuint texture_id;
    GLint texture_width;
    GLint texture_height;
  };

};

#endif