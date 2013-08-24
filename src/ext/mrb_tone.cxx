#include "mrb.hxx"
#include "tone.hxx"

namespace Moon {

  static void moon_mrb_tone_deallocate(mrb_state *mrb, void *p) {
    delete((std::shared_ptr<Tone>*)p);
  };

  const struct mrb_data_type tone_data_type = {
    "Tone", moon_mrb_tone_deallocate,
  };

  static mrb_value moon_mrb_tone_new(mrb_state *mrb, mrb_value klass) {
    mrb_float h, s, l;
    mrb_get_args(mrb, "fff", &h, &s, &l);

    // INCEPTION
    // Vinnie_win │ std::shared_ptr <Object>* p = new std::shared_ptr <Object> (new Object (...));
    // Vinnie_win │ Have you seen the movie Inception? It's particularly appropriate in this case.
    auto tone = new std::shared_ptr<Tone>(new Tone(h, s, l)); // std::make_shared<Tone>(h, s, l) would be better

    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_ptr(klass), &tone_data_type, tone));
  };

  static mrb_value moon_mrb_tone_hue_setter(mrb_state *mrb, mrb_value self) {
    mrb_float hue;
    mrb_get_args(mrb, "f", &hue);

    std::shared_ptr<Tone>* tone;
    Data_Get_Struct(mrb, self, &tone_data_type, tone);

    (*tone)->hue = hue;

    return mrb_float_value(mrb, (*tone)->hue);
  }

  static mrb_value moon_mrb_tone_hue_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Tone>* tone;
    Data_Get_Struct(mrb, self, &tone_data_type, tone);

    return mrb_float_value(mrb, (*tone)->hue);
  }

  static mrb_value moon_mrb_tone_saturation_setter(mrb_state *mrb, mrb_value self) {
    mrb_float saturation;
    mrb_get_args(mrb, "f", &saturation);

    std::shared_ptr<Tone>* tone;
    Data_Get_Struct(mrb, self, &tone_data_type, tone);

    (*tone)->saturation = saturation;

    return mrb_float_value(mrb, (*tone)->saturation);
  }

  static mrb_value moon_mrb_tone_saturation_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Tone>* tone;
    Data_Get_Struct(mrb, self, &tone_data_type, tone);

    return mrb_float_value(mrb, (*tone)->saturation);
  }

  static mrb_value moon_mrb_tone_lightness_setter(mrb_state *mrb, mrb_value self) {
    mrb_float lightness;
    mrb_get_args(mrb, "f", &lightness);

    std::shared_ptr<Tone>* tone;
    Data_Get_Struct(mrb, self, &tone_data_type, tone);

    (*tone)->lightness = lightness;

    return mrb_float_value(mrb, (*tone)->lightness);
  }

  static mrb_value moon_mrb_tone_lightness_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Tone>* tone;
    Data_Get_Struct(mrb, self, &tone_data_type, tone);

    return mrb_float_value(mrb, (*tone)->lightness);
  }

  void moon_mrb_tone_init(mrb_state *mrb) {
    struct RClass *tone_class;
    tone_class = mrb_define_class_under(mrb, mrb_class_get(mrb, "Moon"), "Tone", mrb->object_class);
    MRB_SET_INSTANCE_TT(tone_class, MRB_TT_DATA);

    mrb_define_class_method(mrb, tone_class, "new", moon_mrb_tone_new, MRB_ARGS_REQ(3));
    mrb_define_method(mrb, tone_class, "hue=", moon_mrb_tone_hue_setter, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, tone_class, "hue", moon_mrb_tone_hue_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, tone_class, "saturation=", moon_mrb_tone_saturation_setter, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, tone_class, "saturation", moon_mrb_tone_saturation_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, tone_class, "lightness=", moon_mrb_tone_lightness_setter, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, tone_class, "lightness", moon_mrb_tone_lightness_getter, MRB_ARGS_NONE());
  };

}