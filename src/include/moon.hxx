#ifndef MOON_H
#define MOON_H

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <unordered_map>
#include "unistd.h"
#include "dirent.h"
#include <string.h>
#include <stdexcept>

#include <sys/stat.h>

#ifdef __ANDROID__
  /* OpenGL ES */
  #include <GLES2/gl2.h>
  #include <jni.h>
#else
  /* OpenGL */
  #define GL_GLEXT_PROTOTYPES
  #include <GL/gl.h>
#endif

/* GLM */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* Include the mruby header */
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/compile.h>
#include <mruby/variable.h>
#include <mruby/string.h>

// _WIN32 is set for both 32 and 64 bit
#ifdef _WIN32
#  define stat _stat
#endif

inline bool exists(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
};

#endif