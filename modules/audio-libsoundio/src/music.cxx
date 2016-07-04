#include "moon/intern.h"
#include "moon/audio/libsoundio/music.hxx"

namespace Moon
{
  Music::Music(const std::string filename) :
    file(filename, SFM_READ)
  {
  	printf ("Opened file '%s'\n", filename.c_str()) ;
	printf ("    Sample rate : %d\n", file.samplerate ()) ;
  printf ("    Channels    : %d\n", file.channels ()) ;
  };


  Music::~Music() {
  }


  void Music::mix(struct SoundIoChannelArea *areas, const struct SoundIoChannelLayout &layout, const float sampleRate, unsigned int frames) {
    // TODO: compare file.channels() with layout.channel_count
    // handle mono to stereo, vice versa
    int channels = file.channels();
    float* cache = new float[frames * channels * file.samplerate()];
    auto bytes = file.read(cache, frames);
    printf("Read n bytes: %d\n", bytes);

    printf("file channels: %d\n", channels);
    for (int frame = 0; frame < frames; ++frame) {
      for (size_t channel = 0; channel < channel; ++channel) {
        float* buffer = (float*)(areas[channel].ptr + areas[channel].step * frame);

        float sample = cache[frame * channels + channel];

        // clipping, to avoid overdrive
        *buffer = sample > 1.0f ? 1.0f : (sample < -1.0f ? -1.0f : sample);
      }
    }
    file.seek(frames, SEEK_CUR);
    delete cache;
  }
}
