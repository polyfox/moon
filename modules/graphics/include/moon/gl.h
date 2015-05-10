#ifndef MOON_GL_H
#define MOON_GL_H

/* Some platforms will still require GLEW to be loaded */
#ifdef MOON_GLEW
#  include <GL/glew.h>
#endif

/* OpenGL via GLFW */
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#define GL3_PROTOTYPES
#include <GLFW/glfw3.h>

#endif
