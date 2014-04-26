#if __VERSION__ > 150
layout(location = 0) in vec2 vertex_pos;
layout(location = 1) in vec2 texcoord;
out vec2 f_texcoord;
#else
attribute vec2 vertex_pos;
attribute vec2 texcoord;
varying vec2 f_texcoord;
#endif
uniform mat4 mvp_matrix;

void main(void) {
  gl_Position = mvp_matrix * vec4(vertex_pos, 0.0, 1.0);
  f_texcoord = texcoord;
}