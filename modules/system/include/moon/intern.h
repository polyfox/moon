#ifndef MOON_INTERN_H
#define MOON_INTERN_H

#include <string>
#include <cstring>

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

// _WIN32 is set for both 32 and 64 bit
#ifdef _WIN32
#  define stat _stat
#endif

static inline bool
exists(const std::string& name)
{
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
};

#endif
