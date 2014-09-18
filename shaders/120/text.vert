 #version 120

uniform mat4 mvp_matrix;
attribute vec2 vertex_pos;
attribute vec2 texcoord;
attribute vec4 color;
varying vec2 f_texcoord;
varying vec4 f_color;
void main()
{
  gl_Position = mvp_matrix * vec4(vertex_pos, 0.0, 1.0);
  f_texcoord = texcoord;
  f_color = color;
}
