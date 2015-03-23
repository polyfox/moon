#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include "moon/api.h"
#include "moon/mrb/spritesheet.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/transform.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb_err.hxx"
#include "moon/spritesheet.hxx"
#include "moon/glm.h"

#define IVget(_name_) mrb_iv_get(mrb, self, mrb_intern_lit(mrb, _name_))
#define IVset(_name_, _value_) mrb_iv_set(mrb, self, mrb_intern_lit(mrb, _name_), _value_)

#define KEY_TEXTURE "__texture"

static mrb_sym id_opacity;
static mrb_sym id_tone;
static mrb_sym id_color;
static mrb_sym id_ox;
static mrb_sym id_oy;
static mrb_sym id_angle;
static mrb_sym id_transform;

static void
spritesheet_free(mrb_state *mrb, void *p)
{
  Moon::Spritesheet *spritesheet = (Moon::Spritesheet*)p;
  if (spritesheet) {
    delete(spritesheet);
  }
}

MOON_C_API const struct mrb_data_type spritesheet_data_type = { "Moon::Spritesheet", spritesheet_free };

static inline Moon::Texture*
get_texture(mrb_state *mrb, mrb_value self)
{
  return (Moon::Texture*)mrb_data_get_ptr(mrb, self, &texture_data_type);
}

static inline Moon::Spritesheet*
get_spritesheet(mrb_state *mrb, mrb_value self)
{
  return (Moon::Spritesheet*)mrb_data_get_ptr(mrb, self, &spritesheet_data_type);
}

/*
 * @overload Spritesheet#initialize(texture: Texture, cell_width: int, cell_height: int)
 * @overload Spritesheet#initialize(filename: str, cell_width: int, cell_height: int)
 */
static mrb_value
spritesheet_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_int tile_width, tile_height;
  Moon::Spritesheet *spritesheet;
  mrb_get_args(mrb, "oii", &obj, &tile_width, &tile_height);

  spritesheet = (Moon::Spritesheet*)DATA_PTR(self);
  if (spritesheet) {
    spritesheet_free(mrb, (void*)spritesheet);
  }
  spritesheet = new Moon::Spritesheet();

  switch (mrb_type(obj)) {
    case MRB_TT_STRING: {
      char* filename = RSTRING_PTR(obj);
      if (exists(filename)) {
        mrb_value texture = mmrb_texture_load_file(mrb, filename);
        spritesheet->LoadTexture(get_texture(mrb, texture), tile_width, tile_height);
        IVset(KEY_TEXTURE, texture);
      } else {
        delete(spritesheet);
        mrb_raisef(mrb, E_SCRIPT_ERROR,
                   "cannot load such file -- %S",
                   mrb_str_new_cstr(mrb, filename));
      }
      break;
    }
    case MRB_TT_DATA: {
      if (DATA_TYPE(obj) == &texture_data_type) {
        spritesheet->LoadTexture(get_texture(mrb, obj), tile_width, tile_height);
        IVset(KEY_TEXTURE, obj);
      } else {
        delete(spritesheet);
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "wrong argument DATA type %S (expected Moon::Texture)",
                   mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, obj)));
      }
      break;
    }
    default: {
      delete(spritesheet);
      mrb_raisef(mrb, E_TYPE_ERROR,
                 "wrong argument type %S (expected Moon::Texture or String)",
                 mrb_str_new_cstr(mrb, mrb_obj_classname(mrb, obj)));
    }
  }

  if (!spritesheet) {
    return mrb_nil_value();
  }

  mrb_data_init(self, spritesheet, &spritesheet_data_type);

  return self;
}

static mrb_value
spritesheet_texture_get(mrb_state *mrb, mrb_value self)
{
  return IVget(KEY_TEXTURE);
}

/*
 * @overload Spritesheet#render(x: float, y: float, z: float, index: int)
 * @overload Spritesheet#render(x: float, y: float, z: float, index: int, options: Hash<Symbol, Object>)
 */
static mrb_value
spritesheet_render(mrb_state *mrb, mrb_value self)
{
  mrb_int index;
  mrb_float x, y, z;
  mrb_value options = mrb_nil_value();
  mrb_get_args(mrb, "fffi|H", &x, &y, &z, &index, &options);
  Moon::Spritesheet *spritesheet = get_spritesheet(mrb, self);
  Moon::Spritesheet::RenderState render_op;
  if (!mrb_nil_p(options)) {
    mrb_value keys = mrb_hash_keys(mrb, options);
    int len = mrb_ary_len(mrb, keys);
    const mrb_value *keys_ary = RARRAY_PTR(keys);

    for (int i = 0; i < len; ++i) {
      mrb_value key = keys_ary[i];

      if (mrb_symbol_p(key)) {
        mrb_value val = mrb_hash_get(mrb, options, key);
        // :opacity
        if (mrb_symbol(key) == id_opacity) {
          render_op.opacity = mrb_to_flo(mrb, val);

        // :color
        } else if (mrb_symbol(key) == id_color) {
          render_op.color = mmrb_to_vector4(mrb, val);

        // :tone
        } else if (mrb_symbol(key) == id_tone) {
          render_op.tone = mmrb_to_vector4(mrb, val);

        // :ox
        } else if (mrb_symbol(key) == id_ox) {
          render_op.origin.x = mrb_to_flo(mrb, val);

        // :oy
        } else if (mrb_symbol(key) == id_oy) {
          render_op.origin.y = mrb_to_flo(mrb, val);

        // :angle
        } else if (mrb_symbol(key) == id_angle) {
          render_op.angle = mrb_to_flo(mrb, val);

        // :transform
        } else if (mrb_symbol(key) == id_transform) {
          render_op.transform = mmrb_to_transform(mrb, val);
        }
      }
    }
  }
  spritesheet->Render(x, y, z, index, render_op);
  return mrb_nil_value();
}

/*
 * @return [Integer]
 */
static mrb_value
spritesheet_cell_width(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value((int)get_spritesheet(mrb, self)->tile_width);
}

/*
 * @return [Integer]
 */
static mrb_value
spritesheet_cell_height(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value((int)get_spritesheet(mrb, self)->tile_height);
}

/*
 * @return [Integer]
 */
static mrb_value
spritesheet_cell_count(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value((int)get_spritesheet(mrb, self)->total_sprites);
}

MOON_C_API void
mmrb_spritesheet_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *spritesheet_class;
  spritesheet_class = mrb_define_class_under(mrb, mod, "Spritesheet", mrb->object_class);
  MRB_SET_INSTANCE_TT(spritesheet_class, MRB_TT_DATA);

  mrb_define_method(mrb, spritesheet_class, "initialize",  spritesheet_initialize,     MRB_ARGS_REQ(3));
  mrb_define_method(mrb, spritesheet_class, "render",      spritesheet_render,         MRB_ARGS_ARG(4,1));
  mrb_define_method(mrb, spritesheet_class, "texture",     spritesheet_texture_get,    MRB_ARGS_NONE());
  mrb_define_method(mrb, spritesheet_class, "cell_width",  spritesheet_cell_width,     MRB_ARGS_NONE()); /* deprecated, use width instead */
  mrb_define_method(mrb, spritesheet_class, "cell_height", spritesheet_cell_height,    MRB_ARGS_NONE()); /* deprecated, use height instead */

  mrb_define_method(mrb, spritesheet_class, "cell_count",  spritesheet_cell_count,     MRB_ARGS_NONE());

  mrb_define_method(mrb, spritesheet_class, "width",       spritesheet_cell_width,     MRB_ARGS_NONE());
  mrb_define_method(mrb, spritesheet_class, "height",      spritesheet_cell_height,    MRB_ARGS_NONE());

  id_opacity   = mrb_intern_cstr(mrb, "opacity");
  id_tone      = mrb_intern_cstr(mrb, "tone");
  id_color     = mrb_intern_cstr(mrb, "color");
  id_ox        = mrb_intern_cstr(mrb, "ox");
  id_oy        = mrb_intern_cstr(mrb, "oy");
  id_angle     = mrb_intern_cstr(mrb, "angle");
  id_transform = mrb_intern_cstr(mrb, "transform");
}
