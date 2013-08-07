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

  struct VertexPos {
    GLfloat x;
    GLfloat y;
  };

  struct uvMap {
    GLfloat u;
    GLfloat v;
  };

  struct VertexData2D {
    GLfloat x;
    GLfloat y;
    GLfloat u;
    GLfloat v;
  };

  /* Thankfully, deleting the copy constructor on superclass deletes them
    on the default subclass ones too. 

     Adrinael │ geordi: { C c; C d(c); } struct B { B(const B&) = delete; B() {} };  struct C: B { C() {} };  // example one
       geordi │ error: use of deleted function 'C::C(const C&)'
     Adrinael │ geordi: { C c; C d(c); } struct B { B(const B&) = delete; B() {} };  struct C: B { C(const C&) : B() {}  C() {} };  // and how to get past that
     Adrinael │ There!
     Adrinael │ The default copy ctor will call base class's copy ctor, so in effect it's deleted

    */

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