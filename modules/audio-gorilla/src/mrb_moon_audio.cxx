#include <mruby.h>
#include <mruby/class.h>
#include "moon/gorilla/audio.hxx"
#include "moon/gorilla/mrb/music.hxx"
#include "moon/gorilla/mrb/sound.hxx"

/* Call once per step/frame to update the internal Audio module.
*/
static mrb_value
audio_update(mrb_state *mrb, mrb_value klass)
{
  Moon::Audio::Update();
  return klass;
}

MOON_C_API void
mrb_mruby_moon_audio_gorilla_gem_init(mrb_state* mrb)
{
  struct RClass *moon_module = mrb_define_module(mrb, "Moon");
  struct RClass *audio_module = mrb_define_module_under(mrb, moon_module, "Audio");
  mrb_define_class_method(mrb, audio_module, "update", audio_update, MRB_ARGS_NONE());
  mmrb_music_init(mrb);
  mmrb_sound_init(mrb);
  Moon::Audio::Initialize();
}

MOON_C_API void
mrb_mruby_moon_audio_gorilla_gem_final(mrb_state* mrb)
{
  Moon::Audio::Terminate();
}
