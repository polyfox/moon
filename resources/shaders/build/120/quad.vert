#version 120

attribute vec2 vertex_pos;
attribute vec2 texcoord;
attribute vec4 color;

uniform mat4 mvp_matrix;
varying vec2 f_texcoord;
varying vec4 f_color;

void main(void)
{
    gl_Position = mvp_matrix * vec4(vertex_pos, 0.0, 1.0);
    f_texcoord = texcoord;
    f_color = color;
}
