#include "tone.hxx"

namespace Moon {
  Tone::Tone() {
    hue = 1.0;
    saturation = 1.0;
    lightness = 1.0;
  };

  Tone::Tone(GLfloat hue, GLfloat saturation, GLfloat lightness) {
    this->hue = hue;
    this->saturation = saturation;
    this->lightness = lightness;
  };
};
