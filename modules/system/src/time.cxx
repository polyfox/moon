#include "moon/time.hxx"

// Time here is returned in microseconds

// http://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
#ifdef _WIN32
#  include <windows.h>

static double _frequency = 0;

// https://github.com/glfw/glfw/blob/6d5753c54863877020a4f3d12960e417fb04824b/src/win32_time.c
void moon_time_init() {
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  _frequency = frequency.QuadPart;
}

MOON_C_API unsigned __int64
moon_get_current_time()
{
  LARGE_INTEGER time;
  QueryPerformanceCounter(&time);
  return 1000000 * time.QuadPart / _frequency;
}

#elif __APPLE__
#  include <mach/mach.h>
#  include <mach/mach_time.h>

// https://github.com/glfw/glfw/blob/6d5753c54863877020a4f3d12960e417fb04824b/src/mach_time.c

static double _time_resolution = 0;

void moon_time_init() {
  mach_timebase_info_data_t info;
  mach_timebase_info(&info);
  _time_resolution = info.numer / (info.denom * 1.0e9);
}

MOON_C_API uint64_t
moon_get_current_time()
{
  return mach_absolute_time() * _time_resolution;
}

#else
#  include <time.h>

void moon_time_init() {
  //
}

MOON_C_API uint64_t
moon_get_current_time()
{
  struct timespec spec;
  clock_gettime(CLOCK_MONOTONIC, &spec);
  return spec.tv_sec * 1000000 + spec.tv_nsec / 1000;
}

#endif
