#include "moon/intern.h"
#include "moon/audio/libsoundio/sound.hxx"

namespace Moon
{
  Sound::Sound(const std::string filename)
  {
    SndfileHandle file(filename, SFM_READ);
    printf("Opened file '%s'\n", filename.c_str());
    printf("    Sample rate : %d\n", file.samplerate ());
    printf("    Channels    : %d\n", file.channels ());
    printf("    Frames    : %ld\n", file.frames());

    m_channels = file.channels();
    m_sampleRate = file.samplerate();

    m_currentFrame = 0;

    int totalSamples = file.frames() * m_channels * m_sampleRate;
    m_source = new float[totalSamples];

    m_totalFrames = file.readf(m_source, file.frames());
    printf("total frames: %d\n", m_totalFrames);
    // TODO: shout if totalFrames didn't match file.frames()
  };


  Sound::~Sound() {
    delete[] m_source;
  }

  int Sound::channels() {
    return m_channels;
  }

  int Sound::sampleRate() {
    return m_sampleRate;
  }

  // returns how many frames we actually read
  int Sound::read(float* dst, int frames)
  {
    if (m_currentFrame > m_totalFrames) { return 0; }
    // handle buffer edges (don't point past edge)
    int actual = (m_currentFrame + frames > m_totalFrames) ? m_totalFrames - m_currentFrame : frames;
    memcpy(dst, &m_source[m_currentFrame * m_channels], sizeof(float) * actual * m_channels);

    m_currentFrame += actual;
    return actual;
  }
}
