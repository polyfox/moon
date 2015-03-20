#include <mruby.h>
#include <mruby/class.h>
#include "moon/audio.hxx"
#include "moon/mrb/music.hxx"
#include "moon/mrb/sound.hxx"

static struct RClass *moon_module;
static struct RClass *audio_module;

//static mrb_value
//audio_init(mrb_state *mrb, mrb_value klass)
//{
//  Moon::Audio::Initialize();
//  return klass;
//}

static mrb_value
audio_update(mrb_state *mrb, mrb_value klass)
{
  Moon::Audio::Update();
  return klass;
}

MOON_C_API void
mrb_mruby_moon_audio_gem_init(mrb_state* mrb)
{
  moon_module = mrb_define_module(mrb, "Moon");
  audio_module = mrb_define_module_under(mrb, moon_module, "Audio");
  //mrb_define_class_method(mrb, audio_module, "init", audio_init, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, audio_module, "update", audio_update, MRB_ARGS_NONE());
  mmrb_music_init(mrb, audio_module);
  mmrb_sound_init(mrb, audio_module);
  Moon::Audio::Initialize();
}

MOON_C_API void
mrb_mruby_moon_audio_gem_final(mrb_state* mrb)
{
  Moon::Audio::Terminate();
}
