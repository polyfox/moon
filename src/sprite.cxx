#include "sprite.hxx"

namespace Moon {
  Sprite::Sprite(std::string filename) {
    x = 0;
    y = 0;
    clip = false;

    texture = Texture::load(filename);
  };

  Sprite::~Sprite() {

  };

  void Sprite::render() {
    if(clip) {
      texture->render(x, y, &clip_rect);
    } else {
      texture->render(x, y);
    }
  };
};
