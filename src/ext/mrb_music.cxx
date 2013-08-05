#include "mrb.hxx"
#include "music.hxx"

namespace Moon {

  static mrb_value moon_mrb_music_play(mrb_state *mrb, mrb_value klass) {
    char* filename;
    char* format;
    mrb_get_args(mrb, "zz", &filename, &format);
    return mrb_bool_value(Music::play(filename, format));
  };

  static mrb_value moon_mrb_music_stop(mrb_state *mrb, mrb_value klass) {
    Music::stop();

    return mrb_nil_value();
  };

  static mrb_value moon_mrb_music_is_playing(mrb_state *mrb, mrb_value klass) {
    return mrb_bool_value(Music::is_playing());
  }

  static mrb_value moon_mrb_music_is_stopped(mrb_state *mrb, mrb_value klass) {
    return mrb_bool_value(Music::is_stopped());
  }

  static mrb_value moon_mrb_music_is_finished(mrb_state *mrb, mrb_value klass) {
    return mrb_bool_value(Music::is_finished());
  }

  static mrb_value moon_mrb_music_seek(mrb_state *mrb, mrb_value klass) {
    mrb_int offset;
    mrb_get_args(mrb, "i", &offset);
    return mrb_bool_value(Music::seek(offset));
  }

  static mrb_value moon_mrb_music_pos(mrb_state *mrb, mrb_value klass) {
    return mrb_fixnum_value(Music::pos());
  }

  static mrb_value moon_mrb_music_length(mrb_state *mrb, mrb_value klass) {
    return mrb_fixnum_value(Music::length());
  }

  void moon_mrb_music_init(mrb_state *mrb) {
    //struct RObject *music_class;
    //music_class = (struct RObject*) mrb_obj_alloc(mrb, MRB_TT_OBJECT, mrb->object_class);
    struct RClass *music_class;
    music_class = mrb_define_class(mrb, "Music", mrb->object_class);
    /* Playback */
    mrb_define_class_method(mrb, music_class, "play", moon_mrb_music_play, MRB_ARGS_REQ(2));
    mrb_define_class_method(mrb, music_class, "stop", moon_mrb_music_stop, MRB_ARGS_NONE());

    /* Position */
    mrb_define_class_method(mrb, music_class, "seek",   moon_mrb_music_seek, MRB_ARGS_REQ(1));
    mrb_define_class_method(mrb, music_class, "pos",    moon_mrb_music_pos, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, music_class, "length", moon_mrb_music_length, MRB_ARGS_NONE());

    /* Query */
    mrb_define_class_method(mrb, music_class, "playing?",  moon_mrb_music_is_playing, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, music_class, "stopped?",  moon_mrb_music_is_stopped, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, music_class, "finished?", moon_mrb_music_is_finished, MRB_ARGS_NONE());
    //mrb_define_global_const(mrb, "Music", mrb_obj_value(music_class));
  }

}