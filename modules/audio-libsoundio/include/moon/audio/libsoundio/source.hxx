#ifndef MOON_AUDIO_LIBSOUNDIO_SOURCE_H
#define MOON_AUDIO_LIBSOUNDIO_SOURCE_H

namespace Moon
{
  class Source {
    public:
      Source(const std::string filename);
      SF_INFO info;
      SNDFILE* file;

      bool seekable();
      sf_count_t seek(sf_count_t frames, int whence);
      sf_count_t tell();
      sf_count_t read(float* ptr, sf_count_t frames);
  };
}

#endif
