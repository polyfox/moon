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
    printf("    Frames    : %d\n", file.frames());

    m_channels = file.channels();
    m_sampleRate = file.samplerate();

    currentFrame = 0;

    int totalSamples = file.frames() * m_channels * m_sampleRate;
    source = new float[totalSamples];

    totalFrames = file.readf(source, file.frames());
    printf("total frames: %d\n", totalFrames);
    // TODO: shout if totalFrames didn't match file.frames()
  };


  Sound::~Sound() {
    delete[] source;
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
    if(currentFrame > totalFrames) { return 0; }
    // handle buffer edges (don't point past edge)
    int actual = (currentFrame + frames > totalFrames) ? totalFrames - currentFrame : frames;
    memcpy(dst, &source[currentFrame], sizeof(float) * actual * m_channels * m_sampleRate);

    currentFrame += actual;
    return actual;
  }
}
