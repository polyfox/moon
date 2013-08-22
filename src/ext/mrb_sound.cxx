#include "mrb.hxx"
#include "sound.hxx"

namespace Moon {

  static void moon_mrb_sound_deallocate(mrb_state *mrb, void *p) {
    delete((Sound*)p);
  };

  const struct mrb_data_type sound_data_type = {
    "Sound", moon_mrb_sound_deallocate,
  };

  static mrb_value moon_mrb_sound_new(mrb_state *mrb, mrb_value klass) {
    char* filename;
    char* format;

    mrb_get_args(mrb, "zz", &filename, &format);
    Sound *sound;
    try {
      sound = new Sound(filename, format);
    } catch (std::invalid_argument& e) {
      mrb_raisef(mrb, E_SCRIPT_ERROR, "cannot load such file -- %S", mrb_str_new_cstr(mrb, filename));
    };
    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_ptr(klass), &sound_data_type, sound));
  };

  static mrb_value moon_mrb_sound_play(mrb_state *mrb, mrb_value self) {
    mrb_float gain = 1.0;
    mrb_float pitch = 0.0;
    mrb_float pan = 0.0;
    int argc = mrb_get_args(mrb, "|fff", &gain, &pitch, &pan);

    Sound *sound;
    Data_Get_Struct(mrb, self, &sound_data_type, sound);

    if(argc < 0) {
      return mrb_bool_value(sound->play(gain, pitch, pan));
    } else {
      return mrb_bool_value(sound->play());
    };
  };

  void moon_mrb_sound_init(mrb_state *mrb) {
    struct RClass *sound_class;
    sound_class = mrb_define_class_under(mrb, mrb_class_get(mrb, "Moon"), "Sound", mrb->object_class);
    MRB_SET_INSTANCE_TT(sound_class, MRB_TT_DATA);
    
    mrb_define_class_method(mrb, sound_class, "new", moon_mrb_sound_new, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, sound_class, "play", moon_mrb_sound_play, MRB_ARGS_OPT(3));
  };
  
};