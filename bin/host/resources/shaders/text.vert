uniform mat4 mvp_matrix;

in vec2 vertex_pos;
in vec2 texcoord;
in vec4 color;
out vec2 f_texcoord;
out vec4 f_color;
void main()
{
  gl_Position = mvp_matrix * vec4(vertex_pos, 0.0, 1.0);
  f_texcoord  = texcoord;
  f_color = color;
}