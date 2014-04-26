uniform mat4 mvp_matrix;
#if __VERSION__ > 150
layout(location = 0) in vec2 vertex_pos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec4 color;
out vec2 f_texcoord;
out vec4 f_color;
#else
attribute vec2 vertex_pos;
attribute vec2 texcoord;
attribute vec4 color;
varying vec2 f_texcoord;
varying vec4 f_color;
#endif


void main()
{
  gl_Position = mvp_matrix * vec4(vertex_pos, 0.0, 1.0);
  f_texcoord  = texcoord;
  f_color = color;
}