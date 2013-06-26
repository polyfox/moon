#include "sprite.hxx"

namespace Moon {
  Sprite::Sprite(std::string filename) 
  : texture(filename)
  {
    clip = false;
  };

  Sprite::~Sprite() {

  };

  void Sprite::render() {
    if(clip) {
      texture.render(x, y, &clip_rect);
    } else {
      texture.render(x, y);
    }
  };
};
