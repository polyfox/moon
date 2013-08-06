#ifndef MOON_SPRITESHEET_H
#define MOON_SPRITESHEET_H

#include "moon.hxx"
#include "texture.hxx"

namespace Moon {
  class Spritesheet {
  public:
    GLfloat tileWidth;
    GLfloat tileHeight;

    Spritesheet(std::string filename, int tile_width, int tile_height);
    ~Spritesheet();

    void render(const int &x, const int &y, const float &z, const int &index);
  private:
    bool generate_buffers(); // use only in constructor

    // VBO data
    GLuint  VBO;
    GLuint* mIndexBuffers;

    int totalSprites;

    std::shared_ptr<Texture> texture;
  };
};

#endif