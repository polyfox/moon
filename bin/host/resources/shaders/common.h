#if __GLSL_VERSION__ <= 150
#  define in varying
#  define out attribute
#  define layout(_n_)
#  define def_frag_color
#  define frag_color gl_fragColor
#  define texture texture2D
#else
#  define def_frag_color out vec4 fragColor;
#  define frag_color fragColor
#endif