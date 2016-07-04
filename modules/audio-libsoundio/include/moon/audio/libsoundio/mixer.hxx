#ifndef MOON_AUDIO_LIBSOUNDIO_MIXER_H
#define MOON_AUDIO_LIBSOUNDIO_MIXER_H
#include <soundio/soundio.h>
#include <vector>

class AbstractVoice {
  public:
    bool active;
    int frame;

    virtual float getSample(float sampleRate) {
      return 0.0f;
    };

    virtual void reset() {
      active = false;
      frame = 0;
    }
};

namespace Moon
{
  class Mixer {
    public:
      Mixer();

      void mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames);
      std::vector<AbstractVoice*> voices;
    private:
  };
}

#endif
