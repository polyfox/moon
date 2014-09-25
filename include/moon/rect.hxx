#ifndef MOON_RECT_H
#define MOON_RECT_H

#include "moon/stdh.hxx"

namespace Moon {
  struct Rect {
    Rect() = default;
    Rect(GLint x, GLint y, GLint w, GLint h)  {
      this->x = x;
      this->y = y;
      this->w = w;
      this->h = h;
    }
    GLint x;
    GLint y;
    GLint w;
    GLint h;
  };
};

typedef std::shared_ptr<Moon::Rect> moon_rect;
#define moon_rect_p(mrect) (*mrect)

#endif
