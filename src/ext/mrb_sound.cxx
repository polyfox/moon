#include "mrb.hxx"
#include "audio.hxx"

namespace Moon {
  static void moon_mrb_sound_deallocate(mrb_state *mrb, void *p) {
    delete((ga_Sound*)p);
  };

  const struct mrb_data_type sound_data_type = {
    "Sound", moon_mrb_sound_deallocate,
  };

  static mrb_value
  moon_mrb_sound_initialize(mrb_state *mrb, mrb_value self) {
    char* filename;
    char* format;

    mrb_get_args(mrb, "zz", &filename, &format);
    ga_Sound *sound;

    if (exists(filename)) {
      sound = gau_load_sound_file(filename, format);
    } else {
      mrb_raisef(mrb, E_SCRIPT_ERROR, "cannot load such file -- %S", mrb_str_new_cstr(mrb, filename));
    }

    DATA_TYPE(self) = &sound_data_type;
    DATA_PTR(self) = sound;

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_sound_play(mrb_state *mrb, mrb_value self) {
    mrb_float gain = 1.0;
    mrb_float pitch = 1.0;
    mrb_float pan = 0.0;
    mrb_get_args(mrb, "|fff", &gain, &pitch, &pan);

    ga_Sound *sound;
    Data_Get_Struct(mrb, self, &sound_data_type, sound);

    ga_Handle* handle;
    handle = gau_create_handle_sound(Audio::get_mixer(), sound, &gau_on_finish_destroy, 0, 0);
    ga_handle_setParamf(handle, GA_HANDLE_PARAM_GAIN, gain);
    ga_handle_setParamf(handle, GA_HANDLE_PARAM_PITCH, pitch);
    ga_handle_setParamf(handle, GA_HANDLE_PARAM_PAN, pan);
    ga_handle_play(handle);

    return mrb_nil_value();
  };

  struct RClass*
  moon_mrb_sound_init(mrb_state *mrb) {
    struct RClass *sound_class;
    sound_class = mrb_define_class_under(mrb, moon_module, "Sound", mrb->object_class);
    MRB_SET_INSTANCE_TT(sound_class, MRB_TT_DATA);

    mrb_define_method(mrb, sound_class, "initialize", moon_mrb_sound_initialize, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, sound_class, "play", moon_mrb_sound_play,             MRB_ARGS_OPT(3));

    return sound_class;
  };
};
