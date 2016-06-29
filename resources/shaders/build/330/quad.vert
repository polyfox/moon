#version 330

layout(location = 0) in vec2 vertex_pos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec4 color;

uniform mat4 mvp_matrix;
out vec2 f_texcoord;
out vec4 f_color;

void main(void)
{
    gl_Position = mvp_matrix * vec4(vertex_pos, 0.0, 1.0);
    f_texcoord = texcoord;
    f_color = color;
}
