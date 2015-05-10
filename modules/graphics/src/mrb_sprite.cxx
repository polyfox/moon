#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/numeric.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include "moon/api.h"
#include "moon/mrb/sprite.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/rect.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb_err.hxx"
#include "moon/sprite.hxx"
#include "moon/glm.h"

#define TEXTURE_CLASS mrb_module_get_under(mrb, mrb_module_get(mrb, "Moon"), "Texture")
#define IVget(_name_) mrb_iv_get(mrb, self, mrb_intern_lit(mrb, _name_))
#define IVset(_name_, _value_) mrb_iv_set(mrb, self, mrb_intern_lit(mrb, _name_), _value_)

#define KEY_TEXTURE "__texture"

static struct RClass *sprite_class;

static void
sprite_free(mrb_state *mrb, void *p)
{
  Moon::Sprite *sprite = (Moon::Sprite*)p;
  if (sprite) {
    delete(sprite);
  }
}

MOON_C_API const struct mrb_data_type sprite_data_type = { "Sprite", sprite_free };

static inline Moon::Texture*
get_texture(mrb_state *mrb, mrb_value self)
{
  return (Moon::Texture*)mrb_data_get_ptr(mrb, self, &texture_data_type);
}

static inline Moon::Sprite*
get_sprite(mrb_state *mrb, mrb_value self)
{
  return (Moon::Sprite*)mrb_data_get_ptr(mrb, self, &sprite_data_type);
}

static inline Moon::IntRect*
get_rect(mrb_state *mrb, mrb_value self)
{
  return (Moon::IntRect*)mrb_data_get_ptr(mrb, self, &rect_data_type);
}

static inline Moon::Vector4*
get_vector4(mrb_state *mrb, mrb_value self)
{
  return (Moon::Vector4*)mrb_data_get_ptr(mrb, self, &vector4_data_type);
}

static inline void
cleanup_sprite(mrb_state *mrb, mrb_value self)
{
  void *ptr = DATA_PTR(self);
  if (ptr) {
    sprite_free(mrb, ptr);
  }
  DATA_PTR(self) = NULL;
}

/*
 * @overload Sprite#initialize(texture: Texture)
 * @overload Sprite#initialize(filename: str)
 */
static mrb_value
sprite_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Moon::Sprite *sprite;
  mrb_get_args(mrb, "o", &obj);

  cleanup_sprite(mrb, self);
  sprite = new Moon::Sprite();

  switch (mrb_type(obj)) {
    case MRB_TT_STRING: {
      const char* filename = RSTRING_PTR(obj);
      if (exists(filename)) {
        mrb_value texture = mmrb_texture_load_file(mrb, filename);
        sprite->LoadTexture(get_texture(mrb, texture));
        IVset(KEY_TEXTURE, texture);
      } else {
        delete(sprite);
        mrb_raisef(mrb, E_SCRIPT_ERROR,
                   "cannot load such file -- %S",
                   mrb_str_new_cstr(mrb, filename));
      }
      break;
    }
    case MRB_TT_DATA: {
      if (DATA_TYPE(obj) == &texture_data_type) {
        sprite->LoadTexture(get_texture(mrb, obj));
        IVset(KEY_TEXTURE, obj);
      } else {
        delete(sprite);
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "wrong argument DATA type %S (expected Moon::Texture)",
                   mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, obj)));
      }
      break;
    }
    default: {
      delete(sprite);
      mrb_raisef(mrb, E_TYPE_ERROR,
                 "wrong argument type %S (expected Moon::Texture or String)",
                 mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, obj)));
    }
  }

  if (!sprite) {
    return self;
  }

  mrb_data_init(self, sprite, &sprite_data_type);
  return self;
}

static mrb_value
sprite_render(mrb_state *mrb, mrb_value self)
{
  mrb_float x, y, z;
  mrb_get_args(mrb, "fff", &x, &y, &z);
  get_sprite(mrb, self)->Render(x, y, z);
  return self;
}

static mrb_value
sprite_get_opacity(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_sprite(mrb, self)->opacity);
}

static mrb_value
sprite_set_opacity(mrb_state *mrb, mrb_value self)
{
  mrb_float f;
  mrb_get_args(mrb, "f", &f);
  get_sprite(mrb, self)->opacity = glm::clamp(f, 0.0, 1.0);
  return self;
}

static mrb_value
sprite_get_angle(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_sprite(mrb, self)->angle);
}

static mrb_value
sprite_set_angle(mrb_state *mrb, mrb_value self)
{
  mrb_float f;
  mrb_get_args(mrb, "f", &f);
  get_sprite(mrb, self)->angle = f;
  return self;
}

static mrb_value
sprite_get_ox(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_sprite(mrb, self)->origin.x);
}

static mrb_value
sprite_set_ox(mrb_state *mrb, mrb_value self)
{
  mrb_float i;
  mrb_get_args(mrb, "f", &i);
  get_sprite(mrb, self)->origin.x = i;
  return self;
}

static mrb_value
sprite_get_oy(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_sprite(mrb, self)->origin.y);
}

static mrb_value
sprite_set_oy(mrb_state *mrb, mrb_value self)
{
  mrb_float i;
  mrb_get_args(mrb, "f", &i);
  get_sprite(mrb, self)->origin.y = i;
  return self;
}

static mrb_value
sprite_get_color(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector4_value(mrb, get_sprite(mrb, self)->color);
}

static mrb_value
sprite_set_color(mrb_state *mrb, mrb_value self)
{
  Moon::Vector4 *color;
  Moon::Sprite *sprite = get_sprite(mrb, self);
  mrb_get_args(mrb, "d", &color, &vector4_data_type);
  sprite->color = *color;
  return self;
}

static mrb_value
sprite_get_tone(mrb_state *mrb, mrb_value self)
{
  return mmrb_vector4_value(mrb, get_sprite(mrb, self)->tone);
}

static mrb_value
sprite_set_tone(mrb_state *mrb, mrb_value self)
{
  Moon::Vector4 *tone;
  Moon::Sprite *sprite = get_sprite(mrb, self);
  mrb_get_args(mrb, "d", &tone, &vector4_data_type);
  sprite->tone = *tone;
  return self;
}

static mrb_value
sprite_get_texture(mrb_state *mrb, mrb_value self)
{
  return IVget(KEY_TEXTURE);
}

static mrb_value
sprite_set_texture(mrb_state *mrb, mrb_value self)
{
  mrb_value new_texture;
  Moon::Texture *texture;
  Moon::Sprite *sprite = get_sprite(mrb, self);
  mrb_get_args(mrb, "o", &new_texture);
  texture = get_texture(mrb, new_texture);
  sprite->SetTexture(texture);
  IVset(KEY_TEXTURE, new_texture);
  return self;
}

static mrb_value
sprite_get_clip_rect(mrb_state *mrb, mrb_value self)
{
  Moon::Sprite *sprite = get_sprite(mrb, self);
  if (sprite->use_clip) {
    return mmrb_rect_value(mrb, sprite->GetClipRect());
  }
  return mrb_nil_value();
}

static mrb_value
sprite_set_clip_rect(mrb_state *mrb, mrb_value self)
{
  mrb_value clip_rect;
  Moon::Sprite *sprite = get_sprite(mrb, self);
  mrb_get_args(mrb, "o", &clip_rect);
  if(!mrb_nil_p(clip_rect)) {
    sprite->SetClipRect(*get_rect(mrb, clip_rect));
    sprite->use_clip = true;
  } else {
    sprite->use_clip = false;
  }
  return clip_rect;
}

MOON_C_API void
mmrb_sprite_init(mrb_state *mrb, struct RClass* mod)
{
  sprite_class = mrb_define_class_under(mrb, mod, "Sprite", mrb->object_class);
  MRB_SET_INSTANCE_TT(sprite_class, MRB_TT_DATA);

  mrb_define_method(mrb, sprite_class, "initialize", sprite_initialize,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, sprite_class, "render",     sprite_render,         MRB_ARGS_REQ(3));
  mrb_define_method(mrb, sprite_class, "opacity",    sprite_get_opacity, MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "opacity=",   sprite_set_opacity, MRB_ARGS_REQ(1));


  mrb_define_method(mrb, sprite_class, "angle",      sprite_get_angle,   MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "angle=",     sprite_set_angle,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, sprite_class, "ox",         sprite_get_ox,      MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "ox=",        sprite_set_ox,      MRB_ARGS_REQ(1));
  mrb_define_method(mrb, sprite_class, "oy",         sprite_get_oy,      MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "oy=",        sprite_set_oy,      MRB_ARGS_REQ(1));

  mrb_define_method(mrb, sprite_class, "color",      sprite_get_color,  MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "color=",     sprite_set_color,  MRB_ARGS_REQ(1));

  mrb_define_method(mrb, sprite_class, "tone",       sprite_get_tone,    MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "tone=",      sprite_set_tone,    MRB_ARGS_REQ(1));

  mrb_define_method(mrb, sprite_class, "texture",    sprite_get_texture, MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "texture=",   sprite_set_texture, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, sprite_class, "clip_rect",  sprite_get_clip_rect,    MRB_ARGS_NONE());
  mrb_define_method(mrb, sprite_class, "clip_rect=", sprite_set_clip_rect,    MRB_ARGS_REQ(1));
}
