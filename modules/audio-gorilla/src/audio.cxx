#include "moon/gorilla/audio.hxx"

namespace Moon {
  bool Audio::m_initialized = false;
  gau_Manager* Audio::m_mgr = NULL;
  ga_Mixer* Audio::m_mixer = NULL;
  ga_StreamManager* Audio::m_streamMgr = NULL;

  bool Audio::Initialize() {
    // setup Gorilla Audio
    if (!m_mgr) {
      if (gc_initialize(0) == GC_SUCCESS) {
        m_mgr = gau_manager_create();
        m_mixer = gau_manager_mixer(m_mgr);
        m_streamMgr = gau_manager_streamManager(m_mgr);
      } else {
        return false;
      }
    }
    return true;
  };

  void Audio::Update() {
    if (m_mgr) gau_manager_update(m_mgr);
  };

  void Audio::Terminate() {
    if (m_initialized) {
      Pa_Terminate();
    }
  };

  ga_Mixer* Audio::GetMixer() {
    return m_mixer;
  };

  ga_StreamManager* Audio::GetStreamMgr() {
    return m_streamMgr;
  };
}

