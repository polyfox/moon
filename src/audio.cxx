#include "audio.hxx"


namespace Moon {
  gau_Manager* Audio::mgr = NULL;
  ga_Mixer* Audio::mixer = NULL;
  ga_StreamManager* Audio::streamMgr = NULL;

  void Audio::initialize() {
    // setup Gorilla Audio
    gc_initialize(0);
    mgr = gau_manager_create();

    mixer = gau_manager_mixer(mgr);
    streamMgr = gau_manager_streamManager(mgr);
  };

  void Audio::update() {
    gau_manager_update(mgr);
  };

  void Audio::terminate() {
    gau_manager_destroy(mgr);
    gc_shutdown();
  };

  ga_Mixer* Audio::get_mixer() {
    return mixer;
  };

  ga_StreamManager* Audio::get_stream_mgr() {
    return streamMgr;
  };
}

