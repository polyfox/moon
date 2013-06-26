#ifndef MOON_H
#define MOON_H

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include "unistd.h"
#include "dirent.h"
#include <string.h>

/* OpenGL */
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GLFW/glfw3.h>

/* DevIL */
#include <IL/il.h>
#include <IL/ilu.h>

/* Include the mruby header */
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/compile.h>
#include <mruby/variable.h>
#include <mruby/string.h>

#define StringValueCStr(r) std::string(RSTRING_PTR(r), RSTRING_LEN(r))

#endif