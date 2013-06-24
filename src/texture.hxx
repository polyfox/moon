#ifndef MOON_TEXTURE_H
#define MOON_TEXTURE_H

#include "moon.hxx"

namespace Moon {
  class Texture {
  public:
    Texture(std::string path);
    ~Texture();
    void render(GLfloat x, GLfloat y);
  private:
    //Texture name
    GLuint mTextureID;

    //Texture dimensions
    GLuint mTextureWidth;
    GLuint mTextureHeight;
  };
};

#endif