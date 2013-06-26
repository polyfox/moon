#include "sprite.hxx"

namespace Moon {
  Sprite::Sprite(std::string filename) 
  : texture(filename)
  {
    x = 0;
    y = 0;
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
