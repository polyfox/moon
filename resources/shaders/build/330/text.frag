#version 330

uniform sampler2D tex0;
in vec2 f_texcoord;
in vec4 f_color;
out vec4 fragColor;

void main()
{
    fragColor = vec4(1, 1, 1, texture(tex0, f_texcoord).r) * f_color;
}
