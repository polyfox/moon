#ifndef MOON_SPRITE_H
#define MOON_SPRITE_H

#include "moon.hxx"
#include "texture.hxx"
#include "vertex_buffer.hxx"
#include "graphics.hxx"

namespace Moon {
  class Sprite {
  public:
    Sprite(std::string filename);
    ~Sprite();

    int x;
    int y;
    float z;
    float opacity;
    float angle;
    int ox;
    int oy;
    std::shared_ptr<Color> tone;

    void render();
    std::shared_ptr<Texture> getTexture();
    void setTexture(std::shared_ptr<Texture> tex);

    std::shared_ptr<Rect> getClip();
    void setClip(std::shared_ptr<Rect> clip);
  private:
    bool generate_buffers();

    std::shared_ptr<Rect> clip_rect; // default constructor nullptr
    std::shared_ptr<Shader>  shader;
    std::shared_ptr<Texture> texture;
    VertexBuffer VBO;
  };
};

#endif