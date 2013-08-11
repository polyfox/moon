#include "sound.hxx"

namespace Moon {

  Sound::Sound(std::string filename, std::string format) {
    if (exists(filename)) {
      sound = gau_load_sound_file(filename.c_str(), format.c_str());
    } else {
      throw std::invalid_argument("LoadError: cannot load sound"); //+ filename);
    }
  };

  Sound::~Sound() {
    ga_sound_release(sound);
  };

  bool Sound::play() {
    ga_Handle* handle;

    handle = gau_create_handle_sound(mixer, sound, &gau_on_finish_destroy, 0, 0);
    ga_handle_play(handle);
    return true;
  };

  bool Sound::play(float gain = 1.0, float pitch = 0.0, float pan = 0.0) {
    ga_Handle* handle;

    handle = gau_create_handle_sound(mixer, sound, &gau_on_finish_destroy, 0, 0);
    ga_handle_setParamf(handle, GA_HANDLE_PARAM_GAIN, gain);
    ga_handle_setParamf(handle, GA_HANDLE_PARAM_PITCH, pitch);
    ga_handle_setParamf(handle, GA_HANDLE_PARAM_PAN, pan);
    ga_handle_play(handle);
    return true;
  };

};
