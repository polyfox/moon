#ifndef MOON_TIME_H
#define MOON_TIME_H

#include <stdint.h>

// Time here is returned in microseconds

// http://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
#ifdef _WIN32
#  include <windows.h>

static inline uint64_t
moon_get_current_time()
{
  // TODO: implement
}

#elif __APPLE__
#  include <mach/mach.h>
#  include <mach/mach_time.h>

static inline uint64_t
moon_get_current_time()
{
  // TODO: implement
}

#else
#  include <time.h>

static inline uint64_t
moon_get_current_time()
{
  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  return spec.tv_sec * 1000000 + spec.tv_nsec / 1000;
}

#endif

static inline float
moon_microseconds_to_seconds(uint64_t mics)
{
  return static_cast<float>(mics / 1000000);
}
#endif
