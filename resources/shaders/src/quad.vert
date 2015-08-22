shader_version
var_precision;

/* Vertex attributes */
layout(location = 0) inattr vec2 vertex_pos;
layout(location = 1) inattr vec2 texcoord;
layout(location = 2) inattr vec4 color;

uniform mat4 mvp_matrix;
outvar vec2 f_texcoord;
outvar vec4 f_color;

void main(void) {
  gl_Position = mvp_matrix * vec4(vertex_pos, 0.0, 1.0);
  f_texcoord = texcoord;
  f_color = color;
}
