#include "music.hxx"

namespace Moon {
  Music::Music(std::string filename, std::string format) {
    if (exists(filename)) {
      handle = gau_create_handle_buffered_file(mixer, streamMgr,
                                               filename.c_str(), format.c_str(),
                                               NULL, 0, &loopSrc);
    } else {
      throw std::invalid_argument("LoadError: cannot load Music"); //+ filename);
    }
  }

  Music::~Music() {
    ga_handle_destroy(handle);
  }

  bool Music::setup_loop(int trigger, int target) {
    gau_sample_source_loop_set(loopSrc, trigger, target);
    return true;
  }

  bool Music::clear_loop() {
    gau_sample_source_loop_clear(loopSrc);
    return true;
  }

  bool Music::play(float gain = 1.0, float pitch = 0.0, float pan = 0.0) {
    ga_handle_setParamf(handle, GA_HANDLE_PARAM_GAIN, gain);
    ga_handle_setParamf(handle, GA_HANDLE_PARAM_PITCH, pitch);
    ga_handle_setParamf(handle, GA_HANDLE_PARAM_PAN, pan);
    ga_handle_play(handle);
    return true;
  }

  bool Music::is_playing() {
    return ga_handle_playing(handle);
  }

  bool Music::is_stopped() {
    return ga_handle_stopped(handle);
  }

  bool Music::is_finished() {
    return ga_handle_finished(handle);
  }

  bool Music::seek(int offset) {
    return ga_handle_seek(handle, offset) != 0 ? false : true;
  }

  int Music::pos() {
    return ga_handle_tell(handle, GA_TELL_PARAM_CURRENT);
  }

  int Music::length() {
    return ga_handle_tell(handle, GA_TELL_PARAM_TOTAL);
  }

  void Music::stop() {
    ga_handle_stop(handle);
  }
};
