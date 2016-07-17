#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/audio/libsoundio/mrb/music.hxx"
#include "moon/audio/libsoundio/music.hxx"
#include "moon/api.h"
#include "moon/intern.h"
#include "moon/mrb/helpers.hxx"

static void
music_free(mrb_state *mrb, void *p)
{
  Moon::Music *music = static_cast<Moon::Music*>(p);
  if (music) {
    delete(music);
  }
}

MOON_C_API const struct mrb_data_type music_data_type = { "Moon::Music", music_free };

static mrb_value
music_initialize(mrb_state* mrb, mrb_value self)
{
  char* filename;
  mrb_get_args(mrb, "z", &filename);
  moon_data_cleanup(mrb, self);
  if (exists(filename)) {
    Moon::Music* music = new Moon::Music(std::string(filename));
    mrb_data_init(self, music, &music_data_type);
  } else {
    mrb_raisef(mrb, E_SCRIPT_ERROR,
               "cannot load such file -- %S",
               mrb_str_new_cstr(mrb, filename));
  }
  return self;
}


static mrb_value
music_play(mrb_state* mrb, mrb_value self)
{
  mrb_float gain = 1.0f;
  mrb_float pitch = 1.0f;
  mrb_float pan = 1.0f;
  mrb_get_args(mrb, "|fff", &gain, &pitch, &pan);
  // TODO
  return self;
}

static mrb_value
music_stop(mrb_state* mrb, mrb_value self)
{
  // TODO
  return self;
}

static mrb_value
music_seek(mrb_state* mrb, mrb_value self)
{
  return self;
}

static mrb_value
music_pos(mrb_state* mrb, mrb_value self)
{
  return mrb_fixnum_value(0);
}

static mrb_value
music_length(mrb_state* mrb, mrb_value self)
{
  return mrb_fixnum_value(0);
}

static mrb_value
music_loop(mrb_state* mrb, mrb_value self)
{
  return self;
}

static mrb_value
music_clear_loop(mrb_state* mrb, mrb_value self)
{
  return self;
}

static mrb_value
music_is_playing(mrb_state* mrb, mrb_value self)
{
  // TODO
  return mrb_bool_value(true);
}

static mrb_value
music_is_stopped(mrb_state* mrb, mrb_value self)
{
  // TODO
  return mrb_bool_value(false);
}

static mrb_value
music_is_finished(mrb_state* mrb, mrb_value self)
{
  // TODO
  return mrb_bool_value(false);
}

MOON_C_API void
mmrb_music_init(mrb_state* mrb, struct RClass* mod)
{
  struct RClass *music_class = mrb_define_class_under(mrb, mod, "Music", mrb->object_class);
  MRB_SET_INSTANCE_TT(music_class, MRB_TT_DATA);

  mrb_define_method(mrb, music_class, "initialize", music_initialize, MRB_ARGS_REQ(2));

  /* Playback */
  mrb_define_method(mrb, music_class, "play", music_play, MRB_ARGS_OPT(3));
  mrb_define_method(mrb, music_class, "stop", music_stop, MRB_ARGS_NONE());

  /* Position */
  mrb_define_method(mrb, music_class, "seek",       music_seek,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, music_class, "pos",        music_pos,        MRB_ARGS_NONE());
  mrb_define_method(mrb, music_class, "length",     music_length,     MRB_ARGS_NONE());
  mrb_define_method(mrb, music_class, "loop",       music_loop,       MRB_ARGS_OPT(2));
  mrb_define_method(mrb, music_class, "clear_loop", music_clear_loop, MRB_ARGS_NONE());

  /* Query */
  mrb_define_method(mrb, music_class, "playing?",  music_is_playing,  MRB_ARGS_NONE());
  mrb_define_method(mrb, music_class, "stopped?",  music_is_stopped,  MRB_ARGS_NONE());
  mrb_define_method(mrb, music_class, "finished?", music_is_finished, MRB_ARGS_NONE());
}
