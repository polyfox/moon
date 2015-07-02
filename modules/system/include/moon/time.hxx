#ifndef MOON_TIME_H
#define MOON_TIME_H

#include <stdint.h>
#include "moon/api.h"

#ifdef _WIN32
MOON_C_API unsigned __int64 moon_get_current_time();
#else
MOON_C_API uint64_t moon_get_current_time();
#endif

void moon_time_init();

static inline float
moon_microseconds_to_seconds(uint64_t mics)
{
  return static_cast<float>(mics / 1000000);
}

#endif
