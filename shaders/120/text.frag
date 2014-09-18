 #version 120

uniform sampler2D tex;
varying vec2 f_texcoord;
varying vec4 f_color;

void main() {
  gl_FragColor = vec4(1, 1, 1, texture2D(tex, f_texcoord).r) * f_color;
}
