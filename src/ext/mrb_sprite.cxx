#include "moon.hxx"
#include "sprite.hxx"

namespace Moon {
  static void moon_mrb_sprite_deallocate(mrb_state *mrb, void *p) {
    delete((Sprite*)p);
  };

  static const struct mrb_data_type sprite_data_type = {
    "sprite", moon_mrb_sprite_deallocate,
  };

  static mrb_value moon_mrb_sprite_new(mrb_state *mrb, mrb_value klass) {
    char* filename;
    mrb_get_args(mrb, "z", &filename);

    Sprite *sprite = new Sprite(filename);
    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_ptr(klass), &sprite_data_type, sprite));
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
  };
};