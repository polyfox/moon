#include <mruby.h>
#include <mruby/class.h>
#include "moon/mrb/music.hxx"
#include "moon/mrb/sound.hxx"

static struct RClass *moon_module;

MOON_C_API void
mrb_mruby_moon_audio_gem_init(mrb_state* mrb)
{
  moon_module = mrb_define_module(mrb, "Moon");
  mmrb_music_init(mrb, moon_module);
  mmrb_sound_init(mrb, moon_module);
}

MOON_C_API void
mrb_mruby_moon_audio_gem_final(mrb_state* mrb)
{
}
