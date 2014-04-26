#if __GLSL_VERSION__ <= 150
#  define invar varying
#  define outvar varying
#  define inattr attribute
#  define outattr attribute
#  define layout(_n_)
#  define def_frag_color
#  define frag_color gl_FragColor
#  define texture texture2D
#else
#  define invar in
#  define outvar out
#  define inattr in
#  define outattr out
#  define def_frag_color out vec4 fragColor;
#  define frag_color fragColor
#endif