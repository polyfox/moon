#include "mrb.hxx"
#include "sprite.hxx"

namespace Moon {

  static void moon_mrb_sprite_deallocate(mrb_state *mrb, void *p) {
    delete((Sprite*)p);
  };

  const struct mrb_data_type sprite_data_type = {
    "Sprite", moon_mrb_sprite_deallocate,
  };

  static mrb_value moon_mrb_sprite_new(mrb_state *mrb, mrb_value klass) {
    char* filename;
    mrb_get_args(mrb, "z", &filename);

    mrb_value self, tone;
    Sprite *sprite = new Sprite(filename);
    self = mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_ptr(klass), &sprite_data_type, sprite));

    auto tone_ptr = new std::shared_ptr<Tone>(sprite->tone);
    tone = mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_get(mrb, "Tone"), &tone_data_type, tone_ptr));
    mrb_iv_set(mrb, self, mrb_intern(mrb, "@tone"), tone);

    return self;
  };

  static mrb_value moon_mrb_sprite_render(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    sprite->render();
    return mrb_nil_value();
  };

  static mrb_value moon_mrb_sprite_x_getter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    return mrb_fixnum_value(sprite->x);
  };

  static mrb_value moon_mrb_sprite_x_setter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    mrb_int i;
    mrb_get_args(mrb, "i", &i);
    sprite->x = i;
    return mrb_fixnum_value(i);
  };

  static mrb_value moon_mrb_sprite_y_getter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    return mrb_fixnum_value(sprite->y);
  };

  static mrb_value moon_mrb_sprite_y_setter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    mrb_int i;
    mrb_get_args(mrb, "i", &i);
    sprite->y = i;
    return mrb_fixnum_value(i);
  };

  static mrb_value moon_mrb_sprite_z_getter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    return mrb_float_value(mrb, sprite->z);
  };

  static mrb_value moon_mrb_sprite_z_setter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    mrb_float f;
    mrb_get_args(mrb, "f", &f);
    sprite->z = f;
    return mrb_float_value(mrb, f);
  };

  static mrb_value moon_mrb_sprite_opacity_getter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    return mrb_float_value(mrb, sprite->opacity);
  };

  static mrb_value moon_mrb_sprite_opacity_setter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    mrb_float f;
    mrb_get_args(mrb, "f", &f);
    sprite->opacity = f;
    return mrb_float_value(mrb, f);
  };

  static mrb_value moon_mrb_sprite_tone_getter(mrb_state *mrb, mrb_value self) {
    return mrb_iv_get(mrb, self, mrb_intern(mrb, "@tone"));
  }

  static mrb_value moon_mrb_sprite_tone_setter(mrb_state *mrb, mrb_value self) {
    mrb_value new_tone;
    mrb_get_args(mrb, "o", &new_tone);

    if (strcmp(mrb_obj_classname(mrb, new_tone), "Tone") != 0)
      mrb_raisef(mrb, E_TYPE_ERROR, "expected Tone but recieved %s", mrb_obj_classname(mrb, new_tone));

    mrb_iv_set(mrb, self, mrb_intern(mrb, "@tone"), new_tone);

    // Besides updating the ivar, we need to update the actual sprite->tone:

    // Get the passed-in object's shared_ptr
    std::shared_ptr<Tone>* tone_ptr;
    Data_Get_Struct(mrb, new_tone, &tone_data_type, tone_ptr);

    // Create a new shared_ptr for this instance and overwrite the old one
    ((Sprite*)DATA_PTR(self))->tone = std::shared_ptr<Tone>(*tone_ptr);

    return new_tone;
  }

  void moon_mrb_sprite_init(mrb_state *mrb) {
    struct RClass *sprite_class;
    sprite_class = mrb_define_class(mrb, "Sprite", mrb->object_class);
    MRB_SET_INSTANCE_TT(sprite_class, MRB_TT_DATA);

    mrb_define_class_method(mrb, sprite_class, "new", moon_mrb_sprite_new, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite_class, "render", moon_mrb_sprite_render, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "x", moon_mrb_sprite_x_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "x=", moon_mrb_sprite_x_setter, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite_class, "y", moon_mrb_sprite_y_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "y=", moon_mrb_sprite_y_setter, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite_class, "z", moon_mrb_sprite_z_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "z=", moon_mrb_sprite_z_setter, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite_class, "opacity", moon_mrb_sprite_opacity_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "opacity=", moon_mrb_sprite_opacity_setter, MRB_ARGS_REQ(1));

    mrb_define_method(mrb, sprite_class, "tone", moon_mrb_sprite_tone_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "tone=", moon_mrb_sprite_tone_setter, MRB_ARGS_REQ(1));
  };
};