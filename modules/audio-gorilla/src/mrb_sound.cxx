#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/gorilla/mrb/sound.hxx"
#include "moon/gorilla/audio.hxx"
#include "moon/api.h"
#include "moon/intern.h"

static void
sound_free(mrb_state *mrb, void *p)
{
  ga_Sound *sound = (ga_Sound*)p;
  if (sound) {
    delete(sound);
  }
}

MOON_C_API const struct mrb_data_type sound_data_type = { "Sound", sound_free };

static inline ga_Sound*
get_sound(mrb_state *mrb, mrb_value self)
{
  return (ga_Sound*)mrb_data_get_ptr(mrb, self, &sound_data_type);
}

/* 
 * @param [String] filename path to the sound file
 * @param [String] format type of the file
 * @return [Sound]
 */
static mrb_value
sound_initialize(mrb_state *mrb, mrb_value self)
{
  char* filename;
  char* format;
  ga_Sound *sound;

  mrb_get_args(mrb, "zz", &filename, &format);

  sound = (ga_Sound*)DATA_PTR(self);
  if (sound) {
    sound_free(mrb, (void*)sound);
  }
  DATA_PTR(self) = NULL;

  if (exists(filename)) {
    sound = gau_load_sound_file(filename, format);
  } else {
    mrb_raisef(mrb, E_SCRIPT_ERROR, "cannot load such file -- %S", mrb_str_new_cstr(mrb, filename));
  }

  mrb_data_init(self, sound, &sound_data_type);

  return self;
}

/* Plays the file.
 *
 * @param [Float] gain
 * @param [Float] pitch
 * @param [Float] pan
 * @return [nil]
 */
static mrb_value
sound_play(mrb_state *mrb, mrb_value self)
{
  mrb_float gain = 1.0;
  mrb_float pitch = 1.0;
  mrb_float pan = 0.0;
  ga_Sound *sound = get_sound(mrb, self);
  ga_Handle* handle;
  mrb_get_args(mrb, "|fff", &gain, &pitch, &pan);
  handle = gau_create_handle_sound(Moon::Audio::GetMixer(), sound, &gau_on_finish_destroy, 0, 0);
  ga_handle_setParamf(handle, GA_HANDLE_PARAM_GAIN, gain);
  ga_handle_setParamf(handle, GA_HANDLE_PARAM_PITCH, pitch);
  ga_handle_setParamf(handle, GA_HANDLE_PARAM_PAN, pan);
  ga_handle_play(handle);

  return mrb_nil_value();
}

MOON_C_API void
mmrb_sound_init(mrb_state *mrb)
{
  struct RClass* mod = mrb_define_module(mrb, "Moon");
  /* Sounds are short audio files (usually used for sound effects) that are
   * played back often. We fully read them into memory to avoid expensive disk
   * I/O.
   *
   * Use {Music} if you want to play longer audio/music streams.
   */
  struct RClass *sound_class = mrb_define_class_under(mrb, mod, "Sound", mrb->object_class);
  MRB_SET_INSTANCE_TT(sound_class, MRB_TT_DATA);

  mrb_define_method(mrb, sound_class, "initialize", sound_initialize, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, sound_class, "play", sound_play,             MRB_ARGS_OPT(3));
}

