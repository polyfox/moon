#ifndef MOON_H
#define MOON_H

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <unordered_map>
#include <stdexcept>
#include <cstring>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

/* OpenGL */
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

/* GLM */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// _WIN32 is set for both 32 and 64 bit
#ifdef _WIN32
#  define stat _stat
#endif

inline bool exists(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
};

#endif