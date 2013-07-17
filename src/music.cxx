#include "music.hxx"

namespace Moon {
  ga_Handle* Music::handle = NULL;

  void Music::play(std::string filename, std::string format) {
    if (handle) ga_handle_destroy(handle);
    handle = gau_create_handle_buffered_file(mixer, streamMgr, filename.c_str(), format.c_str(), NULL, 0, 0);
    ga_handle_play(handle);
  };

  void Music::stop() {
    if (handle) ga_handle_stop(handle);
  };

};
