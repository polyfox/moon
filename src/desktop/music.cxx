#include "music.hxx"

namespace Moon {

  ga_Handle* Music::handle = NULL;

  bool Music::play(std::string filename, std::string format) {
    if (handle) ga_handle_destroy(handle);
    if(exists(filename)) {
      handle = gau_create_handle_buffered_file(mixer, streamMgr, 
                                               filename.c_str(), format.c_str(), 
                                               NULL, 0, 0);
      ga_handle_play(handle);
      return true;
    }
    return false;
  };

  bool Music::is_playing() {
    if (handle) {
      return ga_handle_playing(handle);
    } else {
      return false;
    }
  }

  bool Music::is_stopped() {
    if (handle) {
      return ga_handle_stopped(handle);
    } else {
      return false;
    }
  }

  bool Music::is_finished() {
    if (handle) {
      return ga_handle_finished(handle);
    } else {
      return false;
    }
  }

  bool Music::seek(int offset) {
    if (handle) {
      return ga_handle_seek(handle, offset) != 0 ? false : true;
    } else {
      return false;
    }
  }

  int Music::pos() {
    if (handle) {
      return ga_handle_tell(handle, GA_TELL_PARAM_CURRENT);
    } else {
      return 0;
    }
  }

  int Music::length() {
    if (handle) {
      return ga_handle_tell(handle, GA_TELL_PARAM_TOTAL);
    } else {
      return 0;
    }
  }

  void Music::stop() {
    if (handle) ga_handle_stop(handle);
  };

};