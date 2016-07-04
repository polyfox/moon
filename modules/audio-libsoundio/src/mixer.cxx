#include "moon/audio/libsoundio/mixer.hxx"

namespace Moon {
  Mixer::Mixer()  {
  }

  void Mixer::mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames)
  {
  
    for (int frame = 0; frame < frames; ++frame) {
      for (size_t channel = 0; channel < layout.channel_count; ++channel) {
        float* buffer = (float*)(areas[channel].ptr + areas[channel].step * frame);
        float sample = 0.0f;
        for (size_t voiceIndex = 0; voiceIndex < voices.size(); ++voiceIndex) {
          if (voices[voiceIndex]->active) {
            sample += voices[voiceIndex]->getSample(sampleRate);
            voices[voiceIndex]->frame += 1;
          }
        }
        // clipping, to avoid overdrive
        *buffer = sample > 1.0f ? 1.0f : (sample < -1.0f ? -1.0f : sample);
      }
    }
  }
}
