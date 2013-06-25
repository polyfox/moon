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

  class Texture {
  public:
    Texture(std::string filename);
    ~Texture();

    void render(GLfloat x, GLfloat y, Rect *clip=NULL);
  private:
    GLuint mTextureID;
    GLuint mTextureWidth;
    GLuint mTextureHeight;
  };

};

#endif