#include "moon/mrb/spritesheet.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/transform.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb_err.hxx"
#include "moon/spritesheet.hxx"
#include <glm/glm.hpp>
#include "moon/shared_types.hxx"
#include "moon/mrb/shared_types.hxx"

using Moon::Spritesheet;
using Moon::ss_render_options;

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
  Spritesheet *spritesheet = (Spritesheet*)p;
  if (spritesheet) {
    delete(spritesheet);
  }
}

struct mrb_data_type spritesheet_data_type = { "Spritesheet", spritesheet_free };

/*
 * @overload Spritesheet#initialize(texture: Texture, cell_width: int, cell_height: int)
 * @overload Spritesheet#initialize(filename: str, cell_width: int, cell_height: int)
 */
static mrb_value
spritesheet_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  mrb_int tile_width, tile_height;
  Spritesheet *spritesheet;
  mrb_get_args(mrb, "oii", &obj, &tile_width, &tile_height);

  spritesheet = (Spritesheet*)DATA_PTR(self);
  if (spritesheet) {
    spritesheet_free(mrb, (void*)spritesheet);
  }
  spritesheet = new Spritesheet();

  switch (mrb_type(obj)) {
    case MRB_TT_STRING: {
      char* filename = RSTRING_PTR(obj);
      if (exists(filename)) {
        spritesheet->load_file(filename, tile_width, tile_height);
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
        spritesheet->load_texture(moon_texture_p(texture), tile_width, tile_height);
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

  if (!spritesheet) {
    return mrb_nil_value();
  }

  mrb_data_init(self, spritesheet, &spritesheet_data_type);

  return self;
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

  Spritesheet *spritesheet;
  spritesheet = (Spritesheet*)mrb_data_get_ptr(mrb, self, &spritesheet_data_type);

  ss_render_options render_op;
  render_op.color = glm::vec4(1.0, 1.0, 1.0, 1.0);
  render_op.tone = glm::vec4(0.0, 0.0, 0.0, 1.0);
  render_op.opacity = 1.0f;
  render_op.angle = 0.0f;
  render_op.ox = 0.0f;
  render_op.oy = 0.0f;

  if (!mrb_nil_p(options)) {
    mrb_value keys = mrb_hash_keys(mrb, options);
    int len = mrb_ary_len(mrb, keys);
    const mrb_value *keys_ary = RARRAY_PTR(keys);

    for (int i=0; i < len; ++i) {
      mrb_value key = keys_ary[i];

      if (mrb_symbol_p(key)) {
        mrb_value val = mrb_hash_get(mrb, options, key);
        // :opacity
        if (mrb_symbol(key) == id_opacity) {
          render_op.opacity = mrb_to_flo(mrb, val);

        // :color
        } else if (mrb_symbol(key) == id_color) {
          moon_vec4 *color_ptr;
          color_ptr = (moon_vec4*)mrb_data_get_ptr(mrb, val, &vector4_data_type);
          render_op.color = **color_ptr;

        // :tone
        } else if (mrb_symbol(key) == id_tone) {
          moon_vec4 *color_ptr;
          color_ptr = (moon_vec4*)mrb_data_get_ptr(mrb, val, &vector4_data_type);
          render_op.tone = **color_ptr;

        // :ox
        } else if (mrb_symbol(key) == id_ox) {
          render_op.ox = mrb_to_flo(mrb, val);

        // :oy
        } else if (mrb_symbol(key) == id_oy) {
          render_op.oy = mrb_to_flo(mrb, val);

        // :angle
        } else if (mrb_symbol(key) == id_angle) {
          render_op.angle = mrb_to_flo(mrb, val);

        // :transform
        } else if (mrb_symbol(key) == id_transform) {
          moon_mat4 *mat_ptr;
          mat_ptr = (moon_mat4*)mrb_data_get_ptr(mrb, val, &transform_data_type);
          render_op.transform = **mat_ptr;
        }
      }
    }
  }

  spritesheet->render(x, y, z, index, render_op);
  return mrb_nil_value();
}

/*
 * @return [Integer]
 */
static mrb_value
spritesheet_cell_width(mrb_state *mrb, mrb_value self)
{
  Spritesheet *spritesheet;
  spritesheet = (Spritesheet*)mrb_data_get_ptr(mrb, self, &spritesheet_data_type);
  return mrb_fixnum_value((int)spritesheet->tile_width);
}

/*
 * @return [Integer]
 */
static mrb_value
spritesheet_cell_height(mrb_state *mrb, mrb_value self)
{
  Spritesheet *spritesheet;
  spritesheet = (Spritesheet*)mrb_data_get_ptr(mrb, self, &spritesheet_data_type);
  return mrb_fixnum_value((int)spritesheet->tile_height);
}

/*
 * @return [Integer]
 */
static mrb_value
spritesheet_cell_count(mrb_state *mrb, mrb_value self)
{
  Spritesheet *spritesheet;
  spritesheet = (Spritesheet*)mrb_data_get_ptr(mrb, self, &spritesheet_data_type);
  return mrb_fixnum_value((int)spritesheet->total_sprites);
}

struct RClass*
mmrb_spritesheet_init(mrb_state *mrb)
{
  struct RClass *spritesheet_class;
  spritesheet_class = mrb_define_class_under(mrb, mmrb_Moon, "Spritesheet", mrb->object_class);
  MRB_SET_INSTANCE_TT(spritesheet_class, MRB_TT_DATA);

  mrb_define_method(mrb, spritesheet_class, "initialize",  spritesheet_initialize,     MRB_ARGS_REQ(3));
  mrb_define_method(mrb, spritesheet_class, "render",      spritesheet_render,         MRB_ARGS_ARG(4,1));

  mrb_define_method(mrb, spritesheet_class, "cell_width",  spritesheet_cell_width,     MRB_ARGS_NONE());
  mrb_define_method(mrb, spritesheet_class, "cell_height", spritesheet_cell_height,    MRB_ARGS_NONE());
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

  return spritesheet_class;
}
