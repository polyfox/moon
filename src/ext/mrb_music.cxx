#include "moon.hxx"
#include "music.hxx"

namespace Moon {

  static mrb_value moon_mrb_music_play(mrb_state *mrb, mrb_value klass) {
    char* filename;
    char* format;
    mrb_get_args(mrb, "zz", &filename, &format);
    Music::play(filename, format);

    return mrb_nil_value();
  };

  static mrb_value moon_mrb_music_stop(mrb_state *mrb, mrb_value klass) {
    Music::stop();

    return mrb_nil_value();
  };


  void moon_mrb_music_init(mrb_state *mrb) {
    struct RObject *music_class;
    music_class = (struct RObject*) mrb_obj_alloc(mrb, MRB_TT_OBJECT, mrb->object_class);

    mrb_define_singleton_method(mrb, music_class, "play", moon_mrb_music_play, MRB_ARGS_REQ(1));
    mrb_define_singleton_method(mrb, music_class, "stop", moon_mrb_music_stop, MRB_ARGS_NONE());

    mrb_define_global_const(mrb, "Music", mrb_obj_value(music_class));
  };
};