#ifndef MOON_TEXTURE_H
#define MOON_TEXTURE_H

#include "moon.hxx"
#include "cache.hxx"
#include "shader.hxx"
#include <memory>
#include "tone.hxx"

namespace Moon {

  struct Rect {
    GLfloat x;
    GLfloat y;
    GLfloat w;
    GLfloat h;
  };

  struct vec2 {
    GLfloat x;
    GLfloat y;
  };

  struct uvMap {
    GLfloat u;
    GLfloat v;
  };

  struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
  };

  struct vertex {
    vec2  pos;
    uvMap tex_coord;
    Color color;
  };

  class Texture: public Cache<Texture> {
  public:
    ~Texture();

    static std::shared_ptr<Texture> load(std::string filename);
    void render(const GLfloat &x, const GLfloat &y, const GLfloat &z, const GLfloat &opacity, Tone *tone, const GLuint &vboID, const GLuint &iboID);

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