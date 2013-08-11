uniform sampler2D texture;
varying vec2 f_texcoord;
varying vec4 f_color;

void main() {
  gl_FragColor = vec4(1, 1, 1, texture2D(texture, f_texcoord).a) * f_color;
}