#include "mrb.hxx"
#include "mrb_err.hxx"
#include "sprite.hxx"
#include <glm/glm.hpp>

namespace Moon {

  static void moon_mrb_sprite_deallocate(mrb_state *mrb, void *p) {
    delete((Sprite*)p);
  };

  const struct mrb_data_type sprite_data_type = {
    "Sprite", moon_mrb_sprite_deallocate,
  };

  static mrb_value
  moon_mrb_sprite_initialize(mrb_state *mrb, mrb_value self) {
    char* filename;
    mrb_get_args(mrb, "z", &filename);

    mrb_value color, tone, texture, clip;
    Sprite *sprite = new Sprite(filename);

    DATA_TYPE(self) = &sprite_data_type;
    DATA_PTR(self) = sprite;

    auto color_ptr = new std::shared_ptr<glm::vec4>(sprite->color);
    color = mrb_obj_value(Data_Wrap_Struct(mrb, moon_cVector4, &vector4_data_type, color_ptr));
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@color"), color);

    auto tone_ptr = new std::shared_ptr<glm::vec4>(sprite->tone);
    tone = mrb_obj_value(Data_Wrap_Struct(mrb, moon_cVector4, &vector4_data_type, tone_ptr));
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@tone"), tone);

    auto texture_ptr = new std::shared_ptr<Texture>(sprite->getTexture());
    texture = mrb_obj_value(Data_Wrap_Struct(mrb, moon_cTexture, &texture_data_type, texture_ptr));
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@texture"), texture);

    auto clip_ptr = new std::shared_ptr<Rect>(sprite->getClip());
    if (*clip_ptr) { // if shared_ptr is not NULL internally
      clip = mrb_obj_value(Data_Wrap_Struct(mrb, moon_cRect, &rect_data_type, clip_ptr));
    } else {
      clip = mrb_nil_value();
    }
    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@clip"), clip);

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_sprite_render(mrb_state *mrb, mrb_value self) {
    mrb_float x, y, z;
    mrb_get_args(mrb, "fff", &x, &y, &z);

    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);

    sprite->render(x, y, z);
    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_sprite_opacity_getter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    return mrb_float_value(mrb, sprite->opacity);
  };

  static mrb_value
  moon_mrb_sprite_opacity_setter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    mrb_float f;
    mrb_get_args(mrb, "f", &f);
    sprite->opacity = glm::clamp(f, 0.0, 1.0);
    return mrb_float_value(mrb, f);
  };

  static mrb_value
  moon_mrb_sprite_angle_getter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    return mrb_float_value(mrb, sprite->angle);
  };

  static mrb_value
  moon_mrb_sprite_angle_setter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    mrb_float f;
    mrb_get_args(mrb, "f", &f);
    sprite->angle = f;
    return mrb_float_value(mrb, f);
  };

  static mrb_value
  moon_mrb_sprite_ox_getter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    return mrb_fixnum_value(sprite->ox);
  };

  static mrb_value
  moon_mrb_sprite_ox_setter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    mrb_int i;
    mrb_get_args(mrb, "i", &i);
    sprite->ox = i;
    return mrb_fixnum_value(i);
  };

  static mrb_value
  moon_mrb_sprite_oy_getter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    return mrb_fixnum_value(sprite->oy);
  };

  static mrb_value
  moon_mrb_sprite_oy_setter(mrb_state *mrb, mrb_value self) {
    Sprite *sprite;
    Data_Get_Struct(mrb, self, &sprite_data_type, sprite);
    mrb_int i;
    mrb_get_args(mrb, "i", &i);
    sprite->oy = i;
    return mrb_fixnum_value(i);
  };

  static mrb_value
  moon_mrb_sprite_color_getter(mrb_state *mrb, mrb_value self) {
    return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@color"));
  }

  static mrb_value
  moon_mrb_sprite_color_setter(mrb_state *mrb, mrb_value self) {
    mrb_value new_color;
    mrb_get_args(mrb, "o", &new_color);

    moon_mrb_check_class(mrb, new_color, moon_cVector4, false);

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@color"), new_color);

    // Besides updating the ivar, we need to update the actual sprite->color:

    // Get the passed-in object's shared_ptr
    std::shared_ptr<glm::vec4>* color_ptr;
    Data_Get_Struct(mrb, new_color, &vector4_data_type, color_ptr);

    // Create a new shared_ptr for this instance and overwrite the old one
    ((Sprite*)DATA_PTR(self))->color = std::shared_ptr<glm::vec4>(*color_ptr);

    return new_color;
  }

  static mrb_value
  moon_mrb_sprite_tone_getter(mrb_state *mrb, mrb_value self) {
    return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@tone"));
  }

  static mrb_value
  moon_mrb_sprite_tone_setter(mrb_state *mrb, mrb_value self) {
    mrb_value new_tone;
    mrb_get_args(mrb, "o", &new_tone);

    moon_mrb_check_class(mrb, new_tone, moon_cVector4, false);

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@tone"), new_tone);

    // Besides updating the ivar, we need to update the actual sprite->tone:

    // Get the passed-in object's shared_ptr
    std::shared_ptr<glm::vec4>* tone_ptr;
    Data_Get_Struct(mrb, new_tone, &vector4_data_type, tone_ptr);

    // Create a new shared_ptr for this instance and overwrite the old one
    ((Sprite*)DATA_PTR(self))->tone = std::shared_ptr<glm::vec4>(*tone_ptr);

    return new_tone;
  }


  static mrb_value
  moon_mrb_sprite_texture_getter(mrb_state *mrb, mrb_value self) {
    return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@texture"));
  }

  static mrb_value
  moon_mrb_sprite_texture_setter(mrb_state *mrb, mrb_value self) {
    mrb_value new_texture;
    mrb_get_args(mrb, "o", &new_texture);

    moon_mrb_check_class(mrb, new_texture, moon_cTexture, false);

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@texture"), new_texture);

    // Besides updating the ivar, we need to update the actual sprite->texture:

    // Get the passed-in object's shared_ptr
    std::shared_ptr<Texture>* texture_ptr;
    Data_Get_Struct(mrb, new_texture, &texture_data_type, texture_ptr);

    // Create a new shared_ptr for this instance and overwrite the old one
    ((Sprite*)DATA_PTR(self))->setTexture(*texture_ptr);

    return new_texture;
  }

  static mrb_value
  moon_mrb_sprite_clip_getter(mrb_state *mrb, mrb_value self) {
    return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@clip"));
  }

  static mrb_value
  moon_mrb_sprite_clip_setter(mrb_state *mrb, mrb_value self) {
    mrb_value new_clip;
    mrb_get_args(mrb, "o", &new_clip);

    if (!mrb_nil_p(new_clip))
      moon_mrb_check_class(mrb, new_clip, moon_cRect, false);

    mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@clip"), new_clip);

    // Besides updating the ivar, we need to update the actual sprite->clip:
    if(!mrb_nil_p(new_clip)) {
      // Get the passed-in object's shared_ptr
      std::shared_ptr<Rect>* clip_ptr;
      Data_Get_Struct(mrb, new_clip, &rect_data_type, clip_ptr);

      // Create a new shared_ptr for this instance and overwrite the old one
      ((Sprite*)DATA_PTR(self))->setClip(*clip_ptr);
    } else {
      std::shared_ptr<Rect> nilrect;
      ((Sprite*)DATA_PTR(self))->setClip(nilrect);
    }


    return new_clip;
  }


  struct RClass*
  moon_mrb_sprite_init(mrb_state *mrb) {
    struct RClass *sprite_class;
    sprite_class = mrb_define_class_under(mrb, moon_module, "Sprite", mrb->object_class);
    MRB_SET_INSTANCE_TT(sprite_class, MRB_TT_DATA);

    mrb_define_method(mrb, sprite_class, "initialize", moon_mrb_sprite_initialize,     MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite_class, "render",     moon_mrb_sprite_render,         MRB_ARGS_REQ(3));
    mrb_define_method(mrb, sprite_class, "opacity",    moon_mrb_sprite_opacity_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "opacity=",   moon_mrb_sprite_opacity_setter, MRB_ARGS_REQ(1));


    mrb_define_method(mrb, sprite_class, "angle",      moon_mrb_sprite_angle_getter,   MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "angle=",     moon_mrb_sprite_angle_setter,   MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite_class, "ox",         moon_mrb_sprite_ox_getter,      MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "ox=",        moon_mrb_sprite_ox_setter,      MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite_class, "oy",         moon_mrb_sprite_oy_getter,      MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "oy=",        moon_mrb_sprite_oy_setter,      MRB_ARGS_REQ(1));

    mrb_define_method(mrb, sprite_class, "color",       moon_mrb_sprite_color_getter,  MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "color=",      moon_mrb_sprite_color_setter,  MRB_ARGS_REQ(1));

    mrb_define_method(mrb, sprite_class, "tone",       moon_mrb_sprite_tone_getter,    MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "tone=",      moon_mrb_sprite_tone_setter,    MRB_ARGS_REQ(1));

    mrb_define_method(mrb, sprite_class, "texture",    moon_mrb_sprite_texture_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "texture=",   moon_mrb_sprite_texture_setter, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, sprite_class, "clip_rect",  moon_mrb_sprite_clip_getter,    MRB_ARGS_NONE());
    mrb_define_method(mrb, sprite_class, "clip_rect=", moon_mrb_sprite_clip_setter,    MRB_ARGS_REQ(1));

    return sprite_class;
  };
};