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

static inline wchar_t*
char_to_utf8(char* str)
{
#ifdef _WIN32
  int length;

  // get length of converted string in characters
  length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, sizeof(str), NULL, 0);

  wchar_t *name = new wchar_t[length];
  MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str, sizeof(str), name, length);
  return name;
#else
  int length = mbstowcs(0, str, 0);
  wchar_t *text = new wchar_t[length+1];
  mbstowcs(text, str, length+1);
  return text;
#endif
};
#endif
