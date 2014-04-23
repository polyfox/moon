#ifndef MOON_SPRITESHEET_H
#define MOON_SPRITESHEET_H

#include "moon.hxx"
#include "texture.hxx"

namespace Moon {
  class Spritesheet {
  public:
    GLfloat tile_width;
    GLfloat tile_height;
    int total_sprites;

    Spritesheet(std::string filename, int tile_width, int tile_height);
    ~Spritesheet();

    void render(const int &x, const int &y, const float &z, const int &index, const render_options &render_op);
  private:
    VertexBuffer VBO;

    std::shared_ptr<Texture> texture;
    std::shared_ptr<Shader>  shader;

    bool generate_buffers(); // use only in constructor
  };
};

#endif