#include "moon/intern.h"
#include "moon/audio/libsoundio/source.hxx"

namespace Moon
{
  Source::Source(const std::string filename) {
    info.format = 0;
    file = sf_open(filename.c_str(), SFM_READ, &info);

    if (!file) {
      stringstream error;
      error << "Unable to open audio file '"
        << filename <<  "'";
      throw error.str();
    }
  };


  Source::~Source() {
    if (file) sf_close(file);
  }


  sf_count_t Source::seekable() {
    return info.seekable == SF_TRUE;
  }

  sf_count_t Source::seek(sf_count_t frames, int whence) {
    return sf_seek(file, frames, whence);
  }

  sf_count_t Source::tell() {
    return seek(0, SEEK_CUR);
  }

  sf_count_t Source::read(float* ptr, sf_count_t frames) {
    return sf_readf_float(file, ptr, frames);
  }
}
