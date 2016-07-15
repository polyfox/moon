#include "moon/intern.h"
#include "moon/audio/libsoundio/music.hxx"

namespace Moon
{
  Music::Music(const std::string filename) :
    m_file(filename, SFM_READ)
  {
    printf("Opened file '%s'\n", filename.c_str());
    printf("    Sample rate : %d\n", m_file.samplerate());
    printf("    Channels    : %d\n", m_file.channels());
    m_loop.end = length();
  };

  Music::~Music() {
  }

  int Music::length() {
    return m_file.frames();
  }

  int Music::channels() {
    return m_file.channels();
  }

  int Music::sampleRate() {
    return m_file.samplerate();
  }

  // returns how many frames we actually read
  int Music::read(float* dst, int frames)
  {
    // If looping is enabled, AND the file has content
    if (m_loop.enabled && length() > 0) {
      // TODO: Figure out how to prevent file from being read OVER the loop.end
      // If the m_file could somehow report it's current position that would be swell
      int totalReadFrames = m_file.readf(dst, frames);
      const int step = channels() * sizeof(float);
      // While we still have frames left to fill out, keep looping
      while (totalReadFrames < frames) {
        // rewind to the start of the loop
        m_file.seek(m_loop.start, SEEK_SET);
        int diff = frames - totalReadFrames;
        int readFrames = m_file.readf(dst + totalReadFrames * step, diff);
        totalReadFrames += readFrames;
        if (!readFrames) {
          break;
        }
      }
      return totalReadFrames;
    } else {
      return m_file.readf(dst, frames);
    }
  }

  void Music::setLoop(unsigned int loopStart, unsigned int loopEnd)
  {
    assert(loopStart < loopEnd);
    m_loop.enabled = true;
    m_loop.start = loopStart;
    m_loop.end = loopEnd;
  }

  void Music::clearLoop()
  {
    m_loop.enabled = false;
    m_loop.start = 0;
    m_loop.end = length();
  }
}
