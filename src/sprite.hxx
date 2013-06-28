#ifndef MOON_SPRITE_H
#define MOON_SPRITE_H

#include "moon.hxx"
#include "texture.hxx"

namespace Moon {
  class Sprite {
  public:
    Sprite(std::string filename);
    ~Sprite();

    int x;
    int y;
    Rect clip_rect;
    bool clip;

    void render();
  private:
    std::shared_ptr<Texture> texture;
  };
};

#endif