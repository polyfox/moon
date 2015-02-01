#include "moon/mrb/music.hxx"
#include "moon/music.hxx"

using Moon::Audio;
using Moon::Music;

static void
music_free(mrb_state *mrb, void *p)
{
  Music* music = (Music*)p;
  if (music) {
    if (music->handle) {
      ga_handle_destroy(music->handle);
    }
    delete(music);
  }
}

struct mrb_data_type music_data_type = { "Music", music_free };

static mrb_value
music_initialize(mrb_state *mrb, mrb_value self)
{
  char* filename;
  char* format;
  Music *music;

  mrb_get_args(mrb, "zz", &filename, &format);

  music = (Music*)DATA_PTR(self);
  if (music) {
    music_free(mrb, (void*)music);
  }
  music = new Music();

  if (exists(filename)) {
    music->handle = gau_create_handle_buffered_file(Audio::get_mixer(), Audio::get_stream_mgr(),
                                                    filename, format,
                                                    NULL, 0, &(music->loopSrc));
  } else {
    mrb_raisef(mrb, E_SCRIPT_ERROR, "cannot load such file -- %S", mrb_str_new_cstr(mrb, filename));
  }

  mrb_data_init(self, music, &music_data_type);

  return self;
}

static mrb_value
music_play(mrb_state *mrb, mrb_value self)
{
  mrb_float gain = 1.0;
  mrb_float pitch = 1.0;
  mrb_float pan = 0.0;
  mrb_get_args(mrb, "|fff", &gain, &pitch, &pan);

  Music *music;
  music = (Music*)mrb_data_get_ptr(mrb, self, &music_data_type);

  ga_handle_setParamf(music->handle, GA_HANDLE_PARAM_GAIN, gain);
  ga_handle_setParamf(music->handle, GA_HANDLE_PARAM_PITCH, pitch);
  ga_handle_setParamf(music->handle, GA_HANDLE_PARAM_PAN, pan);
  ga_handle_play(music->handle);

  return mrb_nil_value();
};

static mrb_value
music_stop(mrb_state *mrb, mrb_value self)
{
  Music *music;
  music = (Music*)mrb_data_get_ptr(mrb, self, &music_data_type);

  ga_handle_stop(music->handle);

  return mrb_nil_value();
};

static mrb_value
music_is_playing(mrb_state *mrb, mrb_value self)
{
  Music *music;
  music = (Music*)mrb_data_get_ptr(mrb, self, &music_data_type);
  return mrb_bool_value(ga_handle_playing(music->handle));
}

static mrb_value
music_is_stopped(mrb_state *mrb, mrb_value self)
{
  Music *music;
  music = (Music*)mrb_data_get_ptr(mrb, self, &music_data_type);
  return mrb_bool_value(ga_handle_stopped(music->handle));
}

static mrb_value
music_is_finished(mrb_state *mrb, mrb_value self)
{
  Music *music;
  music = (Music*)mrb_data_get_ptr(mrb, self, &music_data_type);
  return mrb_bool_value(ga_handle_finished(music->handle));
}

static mrb_value
music_seek(mrb_state *mrb, mrb_value self)
{
  mrb_int offset;
  Music *music;
  mrb_get_args(mrb, "i", &offset);

  music = (Music*)mrb_data_get_ptr(mrb, self, &music_data_type);
  return mrb_bool_value(ga_handle_seek(music->handle, offset));
}

static mrb_value
music_pos(mrb_state *mrb, mrb_value self)
{
  Music *music;
  music = (Music*)mrb_data_get_ptr(mrb, self, &music_data_type);
  return mrb_fixnum_value(ga_handle_tell(music->handle, GA_TELL_PARAM_CURRENT));
}

static mrb_value
music_length(mrb_state *mrb, mrb_value self)
{
  Music *music;
  music = (Music*)mrb_data_get_ptr(mrb, self, &music_data_type);
  return mrb_fixnum_value(ga_handle_tell(music->handle, GA_TELL_PARAM_TOTAL));
}

static mrb_value
music_loop(mrb_state *mrb, mrb_value self)
{
  mrb_int trigger = -1;
  mrb_int target = 0;
  Music *music;
  mrb_get_args(mrb, "|ii", &trigger, &target);

  music = (Music*)mrb_data_get_ptr(mrb, self, &music_data_type);
  gau_sample_source_loop_set(music->loopSrc, trigger, target);
  return mrb_nil_value();
}

static mrb_value
music_clear_loop(mrb_state *mrb, mrb_value self)
{
  Music *music;
  music = (Music*)mrb_data_get_ptr(mrb, self, &music_data_type);
  gau_sample_source_loop_clear(music->loopSrc);
  return mrb_bool_value(true);
}

struct RClass*
mmrb_music_init(mrb_state *mrb)
{
  struct RClass *music_class;
  music_class = mrb_define_class_under(mrb, mmrb_Moon, "Music", mrb->object_class);
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

  return music_class;
}
