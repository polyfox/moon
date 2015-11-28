#ifndef MOON_MUSIC_H
#define MOON_MUSIC_H

#include "moon/gorilla/audio.hxx"

namespace Moon {
  struct Music {
    Music() : handle(NULL), loopSrc(NULL) { };
    ga_Handle* handle;
    gau_SampleSourceLoop* loopSrc;
  };
};

#endif
