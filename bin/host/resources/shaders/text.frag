#if __VERSION__ > 150
uniform sampler2D tex;
in vec2 f_texcoord;
in vec4 f_color;
out vec4 fragColor;
#else
uniform sampler2D texture;
varying vec2 f_texcoord;
varying vec4 f_color;
#endif

void main() {
#if __VERSION__ > 150
  fragColor = vec4(1, 1, 1, texture(tex, f_texcoord).r) * f_color;
#else
  gl_FragColor = vec4(1, 1, 1, texture2D(texture, f_texcoord).a) * f_color;
#endif
}