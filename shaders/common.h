#if GL_ES
#  define OLD_SKOOL_GLSL 1
#  if __GLSL_VERSION__ == 200
#    define shader_version #version 100
#    define var_precision \
#      ifdef GL_FRAGMENT_PRECISION_HIGH \
#        define var_precision precision highp float; \
#      else \
#        define var_precision precision mediump float; \
#      endif
#  else
#    define shader_version #version 120
#    define var_precision
#    define lowp
#    define mediump
#    define highp
#  endif
#else
#  if __GLSL_VERSION__ <= 150
#    define OLD_SKOOL_GLSL 1
#  else
#    define OLD_SKOOL_GLSL 0
#  endif
#  define shader_version #version __GLSL_VERSION__
#  define var_precision
#  define lowp
#  define mediump
#  define highp
#endif
#if OLD_SKOOL_GLSL
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