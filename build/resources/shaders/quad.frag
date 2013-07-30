varying vec2 f_texcoord;
uniform sampler2D texture;

void main(void) {
  gl_FragColor = texture2D(texture, f_texcoord);
}