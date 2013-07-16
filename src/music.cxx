#include "music.hxx"

namespace Moon {
  Music::Music(std::string filename) {
    ga_Handle* handle;
    handle = gau_create_handle_buffered_file(mixer, streamMgr, "st01no01m_theme.ogg", "ogg", &gau_on_finish_destroy, 0, 0);
  };

  Music::~Music() {

  };

};
