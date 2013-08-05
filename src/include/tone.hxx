#ifndef MOON_TONE_H
#define MOON_TONE_H

#include "moon.hxx"
#include <memory>

namespace Moon {
  class Tone {
  public:
    Tone();
    Tone(GLfloat hue, GLfloat saturation, GLfloat lightness);

    GLfloat hue;
    GLfloat saturation;
    GLfloat lightness;
  };
};

#endif