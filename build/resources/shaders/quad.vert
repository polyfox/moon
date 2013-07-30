attribute vec2 vertex_pos;
attribute vec2 texcoord;
varying vec2 f_texcoord;
uniform mat4 projection_matrix;

void main(void) {
  gl_Position = projection_matrix * vec4(vertex_pos, 0.0, 1.0);
  f_texcoord = texcoord;
}