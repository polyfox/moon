#ifndef MOON_SPRITE_H
#define MOON_SPRITE_H

#include "moon.hxx"
#include "texture.hxx"
#include "vertex_buffer.hxx"
#include "tone.hxx"

namespace Moon {
  class Sprite {
  public:
    Sprite(std::string filename);
    ~Sprite();

    int x;
    int y;
    float z;
    float opacity;
    std::shared_ptr<Tone> tone;
    Rect clip_rect;
    bool clip;

    void render();
  private:
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Shader>  shader;

    VertexBuffer VBO;
  };
};

#endif