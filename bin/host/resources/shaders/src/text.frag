uniform sampler2D tex;
in vec2 f_texcoord;
in vec4 f_color;
def_frag_color


void main() {
  frag_color = vec4(1, 1, 1, texture(tex, f_texcoord).r) * f_color;
}