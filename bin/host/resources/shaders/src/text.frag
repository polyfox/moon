shader_version
var_precision

uniform sampler2D tex;
invar vec2 f_texcoord;
invar vec4 f_color;
def_frag_color

void main() {
  frag_color = vec4(1, 1, 1, texture(tex, f_texcoord).r) * f_color;
}