#include "moon/mrb.hxx"
#include "moon/music.hxx"

#define def static mrb_value

namespace Moon {
  static void moon_mrb_music_deallocate(mrb_state *mrb, void *p) {
    ga_handle_destroy(((Music*)p)->handle);
    delete((Music*)p);
  };

  const struct mrb_data_type music_data_type = {
    "Music", moon_mrb_music_deallocate,
  };

  static mrb_value
  moon_mrb_music_initialize(mrb_state *mrb, mrb_value self) {
    char* filename;
    char* format;

    mrb_get_args(mrb, "zz", &filename, &format);
    Music *music = new Music();

    if (exists(filename)) {
      music->handle = gau_create_handle_buffered_file(Audio::get_mixer(), Audio::get_stream_mgr(),
                                                      filename, format,
                                                      NULL, 0, &(music->loopSrc));
    } else {
      mrb_raisef(mrb, E_SCRIPT_ERROR, "cannot load such file -- %S", mrb_str_new_cstr(mrb, filename));
    }

    DATA_TYPE(self) = &music_data_type;
    DATA_PTR(self) = music;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_music_play(mrb_state *mrb, mrb_value self) {
    mrb_float gain = 1.0;
    mrb_float pitch = 1.0;
    mrb_float pan = 0.0;
    mrb_get_args(mrb, "|fff", &gain, &pitch, &pan);

    Music *music;
    Data_Get_Struct(mrb, self, &music_data_type, music);

    ga_handle_setParamf(music->handle, GA_HANDLE_PARAM_GAIN, gain);
    ga_handle_setParamf(music->handle, GA_HANDLE_PARAM_PITCH, pitch);
    ga_handle_setParamf(music->handle, GA_HANDLE_PARAM_PAN, pan);
    ga_handle_play(music->handle);

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_music_stop(mrb_state *mrb, mrb_value self) {
    Music *music;
    Data_Get_Struct(mrb, self, &music_data_type, music);

    ga_handle_stop(music->handle);

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_music_is_playing(mrb_state *mrb, mrb_value self) {
    Music *music;
    Data_Get_Struct(mrb, self, &music_data_type, music);
    return mrb_bool_value(ga_handle_playing(music->handle));
  }

  static mrb_value
  moon_mrb_music_is_stopped(mrb_state *mrb, mrb_value self) {
    Music *music;
    Data_Get_Struct(mrb, self, &music_data_type, music);
    return mrb_bool_value(ga_handle_stopped(music->handle));
  }

  static mrb_value
  moon_mrb_music_is_finished(mrb_state *mrb, mrb_value self) {
    Music *music;
    Data_Get_Struct(mrb, self, &music_data_type, music);
    return mrb_bool_value(ga_handle_finished(music->handle));
  }

  static mrb_value
  moon_mrb_music_seek(mrb_state *mrb, mrb_value self) {
    mrb_int offset;
    mrb_get_args(mrb, "i", &offset);

    Music *music;
    Data_Get_Struct(mrb, self, &music_data_type, music);
    return mrb_bool_value(ga_handle_seek(music->handle, offset));
  }

  static mrb_value
  moon_mrb_music_pos(mrb_state *mrb, mrb_value self) {
    Music *music;
    Data_Get_Struct(mrb, self, &music_data_type, music);
    return mrb_fixnum_value(ga_handle_tell(music->handle, GA_TELL_PARAM_CURRENT));
  }

  static mrb_value
  moon_mrb_music_length(mrb_state *mrb, mrb_value self) {
    Music *music;
    Data_Get_Struct(mrb, self, &music_data_type, music);
    return mrb_fixnum_value(ga_handle_tell(music->handle, GA_TELL_PARAM_TOTAL));
  }

  static mrb_value
  moon_mrb_music_loop(mrb_state *mrb, mrb_value self) {
    mrb_int trigger = -1;
    mrb_int target = 0;
    mrb_get_args(mrb, "|ii", &trigger, &target);

    Music *music;
    Data_Get_Struct(mrb, self, &music_data_type, music);
    gau_sample_source_loop_set(music->loopSrc, trigger, target);
    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_music_clear_loop(mrb_state *mrb, mrb_value self) {
    Music *music;
    Data_Get_Struct(mrb, self, &music_data_type, music);
    gau_sample_source_loop_clear(music->loopSrc);
    return mrb_bool_value(true);
  }

  struct RClass*
  moon_mrb_music_init(mrb_state *mrb) {
    struct RClass *music_class;
    music_class = mrb_define_class_under(mrb, moon_module, "Music", mrb->object_class);
    MRB_SET_INSTANCE_TT(music_class, MRB_TT_DATA);

    mrb_define_method(mrb, music_class, "initialize", moon_mrb_music_initialize, MRB_ARGS_REQ(2));

    /* Playback */
    mrb_define_method(mrb, music_class, "play", moon_mrb_music_play, MRB_ARGS_OPT(3));
    mrb_define_method(mrb, music_class, "stop", moon_mrb_music_stop, MRB_ARGS_NONE());

    /* Position */
    mrb_define_method(mrb, music_class, "seek",       moon_mrb_music_seek,       MRB_ARGS_REQ(1));
    mrb_define_method(mrb, music_class, "pos",        moon_mrb_music_pos,        MRB_ARGS_NONE());
    mrb_define_method(mrb, music_class, "length",     moon_mrb_music_length,     MRB_ARGS_NONE());
    mrb_define_method(mrb, music_class, "loop",       moon_mrb_music_loop,       MRB_ARGS_OPT(2));
    mrb_define_method(mrb, music_class, "clear_loop", moon_mrb_music_clear_loop, MRB_ARGS_NONE());

    /* Query */
    mrb_define_method(mrb, music_class, "playing?",  moon_mrb_music_is_playing,  MRB_ARGS_NONE());
    mrb_define_method(mrb, music_class, "stopped?",  moon_mrb_music_is_stopped,  MRB_ARGS_NONE());
    mrb_define_method(mrb, music_class, "finished?", moon_mrb_music_is_finished, MRB_ARGS_NONE());

    return music_class;
  }
}
