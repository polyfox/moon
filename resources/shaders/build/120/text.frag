#version 120

uniform sampler2D tex0;
varying vec2 f_texcoord;
varying vec4 f_color;

void main()
{
    gl_FragColor = vec4(1, 1, 1, texture2D(tex0, f_texcoord).r) * f_color;
}
