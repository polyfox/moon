#ifndef MOON_TEXTURE_H
#define MOON_TEXTURE_H

#include "moon.hxx"

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

  class Texture {
  public:
    Texture(std::string filename);
    ~Texture();

    void render(GLfloat x, GLfloat y, Rect *clip=NULL);
  private:
    GLuint mTextureID;
    GLuint mTextureWidth;
    GLuint mTextureHeight;

    GLuint mVBOID;
    GLuint mIBOID;
  };

};

#endif