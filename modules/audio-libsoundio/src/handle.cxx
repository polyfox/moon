#include "moon/intern.h"
#include "moon/audio/libsoundio/music.hxx"

namespace Moon
{
  Handle::Handle(Moon::Source* source) // this can take mixer as arg in the future if we have more than 1
  {
    this->source = source;
    this->pan = 0.0f;
    this->pitch = 1.0f;
    this->gain = 1.0f;
  };


  Handle::~Handle() {
  }


  void Handle::mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames) {
    float pan = this->pan;
    pan = (pan + 1.0f) / 2.0f;
    pan = std::clamp(pan, 0.0f, 1.0f);

    // resample and pitch
    float sampleScale = source->sampleRate() / sampleRate * pitch;

    // TODO: compare source.channels() with layout.channel_count
    int channels = source->channels();
    int totalSamples = frames * channels * source->sampleRate();
    float* chunk = new float[totalSamples];

    int actual = source->read(chunk, frames);
    //printf("Read n frames: %d\n", actual);

    // TODO: mixer should complain if we don't have stereo

    for (int frame = 0; frame < actual; ++frame) {
      float* left = (float*)(areas[0].ptr + areas[0].step * frame);
      float* right = (float*)(areas[1].ptr + areas[1].step * frame);
      // mix the sample!
      int offset = frame * channels * sampleScale;
      *left += chunk[offset] * gain * (1.0f - pan) * 2;
      *right += chunk[offset + ((channels == 1) ? 0 : 1)] * gain * pan * 2;
    }
    delete[] chunk;
  }
}
