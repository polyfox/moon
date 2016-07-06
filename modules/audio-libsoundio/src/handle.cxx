#include "moon/intern.h"
#include "moon/audio/libsoundio/music.hxx"

namespace Moon
{
  Handle::Handle(Moon::Source* source) // this can take mixer as arg in the future if we have more than 1
  {
    this->source = source;
  };


  Handle::~Handle() {
  }


  void Handle::mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames) {
    // TODO: compare source.channels() with layout.channel_count
    // handle mono to stereo, vice versa
    int channels = source->channels();
    printf("    yy    : %d\n", channels);
    int totalSamples = frames * channels * source->sampleRate();
    printf("    totl    : %d\n", totalSamples);
    printf("    aal    : %d\n", source->sampleRate());
    float* chunk = new float[totalSamples];
    // clear the array (in case we read less data than available)
    //memset(chunk, 0, totalSamples);

    int actual = source->read(chunk, frames);
    printf("Read n frames: %d\n", actual);

    // TODO: compare frames with framesRead

    for (int frame = 0; frame < actual; ++frame) {
      for (size_t channel = 0; channel < channels; ++channel) {
        float* buffer = (float*)(areas[channel].ptr + areas[channel].step * frame);
        // mix the sample!
        *buffer += chunk[frame * channels + channel];
      }
    }
    delete[] chunk;
  }
}
