#ifndef MOON_RECT_H
#define MOON_RECT_H

namespace Moon {
  template<class T> struct Rect {
    T x, y, w, h;
    Rect() = default;
    Rect(T _x, T _y, T _w, T _h) {
      x = _x;
      y = _y;
      w = _w;
      h = _h;
    };
    Rect(const Moon::Rect<T> &r) {
      x = r.x;
      y = r.y;
      w = r.w;
      h = r.h;
    };
  };
  typedef Rect<int> IntRect;
  typedef Rect<float> FloatRect;
};

#endif
