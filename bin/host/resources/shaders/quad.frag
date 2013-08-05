varying vec2 f_texcoord;
uniform sampler2D texture;
uniform float opacity;
uniform vec3 tone;

/*
** Hue, saturation, luminance
*/

vec3 RGBtoHSL(vec3 color) {
  vec3 hsl; // init to 0 to avoid warnings ? (and reverse if + remove first part)
  
  float fmin = min(min(color.r, color.g), color.b);    //Min. value of RGB
  float fmax = max(max(color.r, color.g), color.b);    //Max. value of RGB
  float delta = fmax - fmin;             //Delta RGB value

  hsl.z = (fmax + fmin) / 2.0; // Luminance

  if (delta == 0.0) {  //This is a gray, no chroma...
    hsl.x = 0.0;  // Hue
    hsl.y = 0.0;  // Saturation
  } else {             //Chromatic data...
    if (hsl.z < 0.5)
      hsl.y = delta / (fmax + fmin); // Saturation
    else
      hsl.y = delta / (2.0 - fmax - fmin); // Saturation
    
    float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;
    float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;
    float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;

    if (color.r == fmax )
      hsl.x = deltaB - deltaG; // Hue
    else if (color.g == fmax)
      hsl.x = (1.0 / 3.0) + deltaR - deltaB; // Hue
    else if (color.b == fmax)
      hsl.x = (2.0 / 3.0) + deltaG - deltaR; // Hue

    if (hsl.x < 0.0)
      hsl.x += 1.0; // Hue
    else if (hsl.x > 1.0)
      hsl.x -= 1.0; // Hue
  }

  return hsl;
}

float HuetoRGB(float f1, float f2, float hue) {
  if (hue < 0.0)
    hue += 1.0;
  else if (hue > 1.0)
    hue -= 1.0;
  float res;
  if ((6.0 * hue) < 1.0)
    res = f1 + (f2 - f1) * 6.0 * hue;
  else if ((2.0 * hue) < 1.0)
    res = f2;
  else if ((3.0 * hue) < 2.0)
    res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;
  else
    res = f1;
  return res;
}

vec3 HSLtoRGB(vec3 hsl) {
  vec3 rgb;

  if (hsl.y == 0.0) {
    rgb = vec3(hsl.z); // Luminance
  } else {
    float f2;

    if (hsl.z < 0.5)
      f2 = hsl.z * (1.0 + hsl.y);
    else
      f2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);

    float f1 = 2.0 * hsl.z - f2;

    rgb.r = HuetoRGB(f1, f2, hsl.x + (1.0/3.0));
    rgb.g = HuetoRGB(f1, f2, hsl.x);
    rgb.b= HuetoRGB(f1, f2, hsl.x - (1.0/3.0));
  }

  return rgb;
}

void main(void) {
  vec4 basecolor = texture2D(texture, f_texcoord);
  if(basecolor.a == 0.0) discard; // alpha testing
  gl_FragColor = vec4(HSLtoRGB(RGBtoHSL(basecolor.rgb) * tone), basecolor.a * opacity);
}