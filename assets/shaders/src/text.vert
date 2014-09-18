shader_version
var_precision

uniform mat4 mvp_matrix;

layout(location = 0) inattr vec2 vertex_pos;
layout(location = 1) inattr vec2 texcoord;
layout(location = 2) inattr vec4 color;
outvar vec2 f_texcoord;
outvar vec4 f_color;

void main()
{
  gl_Position = mvp_matrix * vec4(vertex_pos, 0.0, 1.0);
  f_texcoord  = texcoord;
  f_color = color;
}