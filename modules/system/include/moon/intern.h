#ifndef MOON_INTERN_H
#define MOON_INTERN_H

#include <string>
#include <cstring>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

// _WIN32 is set for both 32 and 64 bit
#ifdef _WIN32
#  define stat _stat
#endif

#ifndef M_PI
	#define M_PI 3.1415926535f
#endif

static inline bool
exists(const std::string& name)
{
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
};

static inline void*
moon_mallocset(size_t len, int value)
{
	void* ptr = malloc(len);
	memset(ptr, value, len);
	return ptr;
}

static inline void*
moon_malloczero(size_t len)
{
	return moon_mallocset(len, 0);
}

#endif
