#include "moon/audio.hxx"

namespace Moon {
  gau_Manager* Audio::m_mgr = NULL;
  ga_Mixer* Audio::m_mixer = NULL;
  ga_StreamManager* Audio::m_streamMgr = NULL;

  void Audio::Initialize() {
    // setup Gorilla Audio
    gc_initialize(0);
    m_mgr = gau_manager_create();
    m_mixer = gau_manager_mixer(m_mgr);
    m_streamMgr = gau_manager_streamManager(m_mgr);
  };

  void Audio::Update() {
    gau_manager_update(m_mgr);
  };

  void Audio::Terminate() {
    gau_manager_destroy(m_mgr);
    gc_shutdown();
  };

  ga_Mixer* Audio::GetMixer() {
    return m_mixer;
  };

  ga_StreamManager* Audio::GetStreamMgr() {
    return m_streamMgr;
  };
}

