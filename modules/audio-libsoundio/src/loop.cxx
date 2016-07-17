#include "moon/intern.h"
#include "moon/audio/libsoundio/music.hxx"

namespace Moon
{
  Loop::Loop(Moon::Source* source, std::uint32_t trigger, std::uint32_t target) {
    this->source = source;
    this->trigger = trigger;
    this->target = target;

    // TODO: bounds checking (trigger/target being further than file length)
  };

  Loop::~Loop() {
  }

  int Loop::channels() {
    return source->channels();
  }

  int Loop::sampleRate() {
    return source->sampleRate();
  }

  // returns how many frames we actually read
  int Loop::read(float* dst, int frames)
  {
    std::uint32_t current = source->tell();
    std::uint32_t diff = trigger - current;

    // frames fit without seeking
    if (frames <= diff) {
      return source->read(dst, frames);
    } else { // we need to read then seek
      source->read(dst, diff);
      source->seek(target);
      dst += diff * source->channels();
      source->read(dst, frames);
    }
    return frames;
  }

  // seeks to a given offset (in frames) from the start of the file
  std::uint32_t Loop::seek(std::uint32_t pos) {
    return source->seek(pos);
  }
}
