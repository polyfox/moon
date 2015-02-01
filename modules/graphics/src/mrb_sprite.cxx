#include "moon/mrb/sprite.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/rect.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb_err.hxx"
#include "moon/sprite.hxx"
#include <glm/glm.hpp>
#include "moon/mrb/shared_types.hxx"

using Moon::Sprite;

static void
sprite_free(mrb_state *mrb, void *p)
{
  Sprite *sprite = (Sprite*)p;
  if (sprite) {
    delete(sprite);
  }
}

struct mrb_data_type sprite_data_type = { "Sprite", sprite_free };

/*
 * @overload Sprite#initialize(texture: Texture)
 * @overload Sprite#initialize(filename: str)
 */
static mrb_value
sprite_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value obj, texture, color, tone, clip;
  Sprite *sprite;
  mrb_get_args(mrb, "o", &obj);

  sprite = (Sprite*)DATA_PTR(self);
  if (sprite) {
    sprite_free(mrb, (void*)sprite);
  }
  sprite = new Sprite();

  switch (mrb_type(obj)) {
    case MRB_TT_STRING: {
      char* filename = RSTRING_PTR(obj);
      if (exists(filename)) {
        sprite->load_file(filename);
      } else {
        mrb_raisef(mrb, E_SCRIPT_ERROR,
                   "cannot load such file -- %S",
                   mrb_str_new_cstr(mrb, filename));
      }
      break;
    }
    case MRB_TT_DATA: {
      if (DATA_TYPE(obj) == &texture_data_type) {
        moon_texture *texture;
        texture = (moon_texture*)mrb_data_get_ptr(mrb, obj, &texture_data_type);
        sprite->load_texture(moon_texture_p(texture));
      } else {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "wrong argument DATA type %S (expected Moon::Texture)",
                   mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, obj)));
      }
      break;
    }
    default: {
      mrb_raisef(mrb, E_TYPE_ERROR,
                 "wrong argument type %S (expected Moon::Texture or String)",
                 mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, obj)));
    }
  }

  if (!sprite) {
    return mrb_nil_value();
  }

  mrb_data_init(self, sprite, &sprite_data_type);

  color = mmrb_vector4_wrap(mrb, sprite->color);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@color"), color);

  tone = mmrb_vector4_wrap(mrb, sprite->tone);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@tone"), tone);

  auto texture_ptr = new moon_texture(sprite->getTexture());
  texture = mrb_obj_value(Data_Wrap_Struct(mrb, mmrb_Texture, &texture_data_type, texture_ptr));
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@texture"), texture);

  auto clip_ptr = new moon_rect(sprite->getClip());
  if (*clip_ptr) { // if shared_ptr is not NULL internally
    clip = mrb_obj_value(Data_Wrap_Struct(mrb, mmrb_Rect, &rect_data_type, clip_ptr));
  } else {
    clip = mrb_nil_value();
  }
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@clip"), clip);

  return self;
}

static mrb_value
sprite_render(mrb_state *mrb, mrb_value self)
{
  mrb_float x, y, z;
  mrb_get_args(mrb, "fff", &x, &y, &z);

  Sprite *sprite;
  sprite = (Sprite*)mrb_data_get_ptr(mrb, self, &sprite_data_type);

  sprite->render(x, y, z);
  return mrb_nil_value();
}

static mrb_value
sprite_opacity_getter(mrb_state *mrb, mrb_value self)
{
  Sprite *sprite;
  sprite = (Sprite*)mrb_data_get_ptr(mrb, self, &sprite_data_type);
  return mrb_float_value(mrb, sprite->opacity);
}

static mrb_value
sprite_opacity_setter(mrb_state *mrb, mrb_value self)
{
  Sprite *sprite;
  sprite = (Sprite*)mrb_data_get_ptr(mrb, self, &sprite_data_type);
  mrb_float f;
  mrb_get_args(mrb, "f", &f);
  sprite->opacity = glm::clamp(f, 0.0, 1.0);
  return mrb_float_value(mrb, f);
}

static mrb_value
sprite_angle_getter(mrb_state *mrb, mrb_value self)
{
  Sprite *sprite;
  sprite = (Sprite*)mrb_data_get_ptr(mrb, self, &sprite_data_type);
  return mrb_float_value(mrb, sprite->angle);
}

static mrb_value
sprite_angle_setter(mrb_state *mrb, mrb_value self)
{
  Sprite *sprite;
  sprite = (Sprite*)mrb_data_get_ptr(mrb, self, &sprite_data_type);
  mrb_float f;
  mrb_get_args(mrb, "f", &f);
  sprite->angle = f;
  return mrb_float_value(mrb, f);
}

static mrb_value
sprite_ox_getter(mrb_state *mrb, mrb_value self)
{
  Sprite *sprite;
  sprite = (Sprite*)mrb_data_get_ptr(mrb, self, &sprite_data_type);
  return mrb_fixnum_value(sprite->ox);
}

static mrb_value
sprite_ox_setter(mrb_state *mrb, mrb_value self)
{
  Sprite *sprite;
  sprite = (Sprite*)mrb_data_get_ptr(mrb, self, &sprite_data_type);
  mrb_int i;
  mrb_get_args(mrb, "i", &i);
  sprite->ox = i;
  return mrb_fixnum_value(i);
}

static mrb_value
sprite_oy_getter(mrb_state *mrb, mrb_value self)
{
  Sprite *sprite;
  sprite = (Sprite*)mrb_data_get_ptr(mrb, self, &sprite_data_type);
  return mrb_fixnum_value(sprite->oy);
}

static mrb_value
sprite_oy_setter(mrb_state *mrb, mrb_value self)
{
  Sprite *sprite;
  sprite = (Sprite*)mrb_data_get_ptr(mrb, self, &sprite_data_type);
  mrb_int i;
  mrb_get_args(mrb, "i", &i);
  sprite->oy = i;
  return mrb_fixnum_value(i);
}

static mrb_value
sprite_color_getter(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@color"));
}

static mrb_value
sprite_color_setter(mrb_state *mrb, mrb_value self)
{
  mrb_value new_color;
  mrb_get_args(mrb, "o", &new_color);

  mmrb_check_class(mrb, new_color, mmrb_Vector4, false);

  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@color"), new_color);

  // Besides updating the ivar, we need to update the actual sprite->color:

  // Get the passed-in object's shared_ptr
  moon_vec4* color_ptr;
  color_ptr = (moon_vec4*)mrb_data_get_ptr(mrb, new_color, &vector4_data_type);

  // Create a new shared_ptr for this instance and overwrite the old one
  ((Sprite*)DATA_PTR(self))->color = moon_vec4(*color_ptr);

  return new_color;
}

static mrb_value
sprite_tone_getter(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@tone"));
}

static mrb_value
sprite_tone_setter(mrb_state *mrb, mrb_value self)
{
  mrb_value new_tone;
  mrb_get_args(mrb, "o", &new_tone);

  mmrb_check_class(mrb, new_tone, mmrb_Vector4, false);

  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@tone"), new_tone);

  // Besides updating the ivar, we need to update the actual sprite->tone:

  // Get the passed-in object's shared_ptr
  moon_vec4* tone_ptr;
  tone_ptr = (moon_vec4*)mrb_data_get_ptr(mrb, new_tone, &vector4_data_type);

  // Create a new shared_ptr for this instance and overwrite the old one
  ((Sprite*)DATA_PTR(self))->tone = moon_vec4(*tone_ptr);

  return new_tone;
}

static mrb_value
sprite_texture_getter(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@texture"));
}

static mrb_value
sprite_texture_setter(mrb_state *mrb, mrb_value self)
{
  mrb_value new_texture;
  mrb_get_args(mrb, "o", &new_texture);

  mmrb_check_class(mrb, new_texture, mmrb_Texture, false);

  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@texture"), new_texture);

  // Besides updating the ivar, we need to update the actual sprite->texture:

  // Get the passed-in object's shared_ptr
  moon_texture* texture_ptr;
  texture_ptr = (moon_texture*)mrb_data_get_ptr(mrb, new_texture, &texture_data_type);

  // Create a new shared_ptr for this instance and overwrite the old one
  ((Sprite*)DATA_PTR(self))->setTexture(*texture_ptr);

  return new_texture;
}

static mrb_value
sprite_clip_getter(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "@clip"));
}

static mrb_value
sprite_clip_setter(mrb_state *mrb, mrb_value self)
{
  mrb_value new_clip;
  mrb_get_args(mrb, "o", &new_clip);

  if (!mrb_nil_p(new_clip))
    mmrb_check_class(mrb, new_clip, mmrb_Rect, false);

  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "@clip"), new_clip);

  // Besides updating the ivar, we need to update the actual sprite->clip:
  if(!mrb_nil_p(new_clip)) {
    // Get the passed-in object's shared_ptr
    moon_rect* clip_ptr;
    clip_ptr = (moon_rect*)mrb_data_get_ptr(mrb, new_clip, &rect_data_type);

    // Create a new shared_ptr for this instance and overwrite the old one
    ((Sprite*)DATA_PTR(self))->setClip(*clip_ptr);
  } else {
    moon_rect nilrect;
    ((Sprite*)DATA_PTR(self))->setClip(nilrect);
  }

  return new_clip;
}

struct RClass*
mmrb_sprite_init(mrb_state *mrb)
{
  struct RClass *sprite_class;
  sprite_class = mrb_define_class_under(mrb, mmrb_Moon, "Sprite", mrb->object_class);
  MRB_SET_INSTANCE_TT(sprite_class, MRB_TT_DATA);

  mrb_define_method(mrb, sprite_class, "initialize", sprite_initialize,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, sprite_class, "render",     sprite_render,         MRB_ARGS_REQ(3));
  mrb_define_method(mrb, sprite_class, "opacity",    sprite_opacity_getter, MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "opacity=",   sprite_opacity_setter, MRB_ARGS_REQ(1));


  mrb_define_method(mrb, sprite_class, "angle",      sprite_angle_getter,   MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "angle=",     sprite_angle_setter,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, sprite_class, "ox",         sprite_ox_getter,      MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "ox=",        sprite_ox_setter,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, sprite_class, "oy",         sprite_oy_getter,      MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "oy=",        sprite_oy_setter,      MRB_ARGS_REQ(1));

  mrb_define_method(mrb, sprite_class, "color",      sprite_color_getter,  MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "color=",     sprite_color_setter,  MRB_ARGS_REQ(1));

  mrb_define_method(mrb, sprite_class, "tone",       sprite_tone_getter,    MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "tone=",      sprite_tone_setter,    MRB_ARGS_REQ(1));

  mrb_define_method(mrb, sprite_class, "texture",    sprite_texture_getter, MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "texture=",   sprite_texture_setter, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, sprite_class, "clip_rect",  sprite_clip_getter,    MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "clip_rect=", sprite_clip_setter,    MRB_ARGS_REQ(1));

  return sprite_class;
}
