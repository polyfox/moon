#ifndef MOON_STREAM_H
#define MOON_STREAM_H

#include "moon/audio.hxx"

namespace Moon {
  struct Stream {
    Stream() : handle(NULL), loopSrc(NULL) { };
    ga_Handle* handle;
    gau_SampleSourceLoop* loopSrc;
  };
};

#endif
