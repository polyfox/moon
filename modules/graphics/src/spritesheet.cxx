#include "moon/engine.hxx"
#include "moon/spritesheet.hxx"
#include "moon/texture.hxx"
#include "moon/vector4.hxx"

namespace Moon {
  Spritesheet::Spritesheet() : m_vbo(GL_STATIC_DRAW)
  {
    shader = NULL;
  }

  Spritesheet::~Spritesheet() {

  };

};
