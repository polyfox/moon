#ifndef MOON_SPRITESHEET_H
#define MOON_SPRITESHEET_H

#include "moon.hxx"
#include "texture.hxx"

namespace Moon {
  class Spritesheet {
  public:
    GLfloat tile_width;
    GLfloat tile_height;

    Spritesheet(std::string filename, int tile_width, int tile_height);
    ~Spritesheet();

    void render(const int &x, const int &y, const float &z, const int &index);
  private:
    bool generate_buffers(); // use only in constructor

    // VBO data
    GLuint  VBO;
    GLuint* index_buffers;

    int total_sprites;

    std::shared_ptr<Texture> texture;
  };
};

#endif