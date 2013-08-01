#include "sprite.hxx"

namespace Moon {
  Sprite::Sprite(std::string filename) {
    x = 0;
    y = 0;
    z = 0.0;
    opacity = 1.0;
    clip = false;

    texture = Texture::load(filename);
  };

  Sprite::~Sprite() {

  };

  void Sprite::render() {
    if(clip) {
      texture->render(x, y, z, opacity, &clip_rect);
    } else {
      texture->render(x, y, z, opacity);
    }
  };
};
