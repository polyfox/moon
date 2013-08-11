uniform mat4 mvp_matrix;

layout(location=0) attribute vec2 vertex_pos;
layout(location=1) attribute vec2 tex_coord;
layout(location=2) attribute vec4 color;
varying vec2 f_texcoord;
varying vec4 f_color;
void main()
{
  gl_Position = mvp_matrix * vec4(vertex_pos, 0.0, 1.0);
  f_texcoord  = tex_coord;
  f_color = color;
}