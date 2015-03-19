#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include "moon/api.h"
#include "moon/mrb/font.hxx"
#include "moon/mrb/transform.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/glm.h"
#include "moon/font.hxx"

static mrb_sym id_outline;
static mrb_sym id_outline_color;
static mrb_sym id_transform;

static void
font_free(mrb_state *mrb, void *p)
{
  Moon::Font *font = (Moon::Font*)p;
  if (font) {
    delete(font);
  }
}

MOON_C_API const struct mrb_data_type font_data_type = { "Font", font_free };

static inline Moon::Font*
get_font(mrb_state *mrb, mrb_value self)
{
  return (Moon::Font*)mrb_data_get_ptr(mrb, self, &font_data_type);
}

static inline Moon::Vector4*
get_vector4(mrb_state *mrb, mrb_value self)
{
  return (Moon::Vector4*)mrb_data_get_ptr(mrb, self, &vector4_data_type);
}

static inline Moon::Transform*
get_transform(mrb_state *mrb, mrb_value self)
{
  return (Moon::Transform*)mrb_data_get_ptr(mrb, self, &transform_data_type);
}

static mrb_value
font_initialize(mrb_state *mrb, mrb_value self)
{
  char* filename;
  mrb_int font_size;
  Moon::Font *font;
  mrb_get_args(mrb, "zi", &filename, &font_size);

  font = (Moon::Font*)DATA_PTR(self);
  if (font) {
    font_free(mrb, (void*)font);
  }

  if (exists(std::string(filename))) {
    font = new Moon::Font(filename, font_size);
    mrb_data_init(self, font, &font_data_type);
  } else {
    mrb_raisef(mrb, E_SCRIPT_ERROR,
                   "cannot load such file -- %S",
                   mrb_str_new_cstr(mrb, filename));
  }


  return self;
}

/*
 * Font#draw_text(x, y, z, str[, color])
 * @param [Numeric] x
 * @param [Numeric] y
 * @param [Numeric] z
 * @param [String] str
 * @param [Vector4] color
 *   @optional
 */
static mrb_value
font_render(mrb_state *mrb, mrb_value self)
{
  mrb_float x, y, z;
  mrb_value color = mrb_nil_value();
  mrb_value options = mrb_nil_value();
  char* str;
  mrb_get_args(mrb, "fffz|oH", &x, &y, &z, &str, &color, &options);

  // convert to wide char (UTF-8)
  wchar_t *text = char_to_utf8(str);

  Moon::Font *font = get_font(mrb, self);

  if (!mrb_nil_p(options)) {
    Moon::font_render_options render_op;

    mrb_value keys = mrb_hash_keys(mrb, options);
    int len = mrb_ary_len(mrb, keys);
    const mrb_value *keys_ary = RARRAY_PTR(keys);

    for (int i=0; i < len; ++i) {
      mrb_value key = keys_ary[i];

      if (mrb_symbol_p(key)) {
        // :opacity
        mrb_value val = mrb_hash_get(mrb, options, key);
        if (mrb_symbol(key) == id_outline) {
          render_op.outline = mrb_to_flo(mrb, val);
        } else if (mrb_symbol(key) == id_outline_color) {
          render_op.outline_color = *get_vector4(mrb, val);
        } else if (mrb_symbol(key) == id_transform) {
          render_op.transform = *get_transform(mrb, val);
        }
      }
    }
    if(!mrb_nil_p(color)) {
      render_op.color = *get_vector4(mrb, color);
    }
    font->draw_text(x, y, z, text, render_op);
  } else {
    if(!mrb_nil_p(color)) {
      //text_color needs to be dereferenced to shared_ptr first and then to value
      font->draw_text(x, y, z, text, *get_vector4(mrb, color));
    } else {
      font->draw_text(x, y, z, text);
    }
  }
  delete[] text;

  return mrb_nil_value();
}

/*
 * Size of the font.
 * @return [Integer]
 */
static mrb_value
font_size(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_font(mrb, self)->size());
}

/*
 * Calculates the string's width and height in pixel using the font.
 * @param [String] str
 * @return [Array<Integer>[2]]
 */
static mrb_value
font_calc_bounds(mrb_state *mrb, mrb_value self)
{
  char *str;
  mrb_get_args(mrb, "z", &str);
  // convert to wide char (UTF-8)
  wchar_t *text = char_to_utf8(str);
  Moon::Font *font = get_font(mrb, self);
  Moon::Vector2 bounds = font->compute_string_bbox(text);
  delete[] text;
  mrb_value argv[2] = { mrb_fixnum_value(bounds.x), mrb_fixnum_value(bounds.y) };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

MOON_C_API void
mmrb_font_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *font_class;
  font_class = mrb_define_class_under(mrb, mod, "Font", mrb->object_class);
  MRB_SET_INSTANCE_TT(font_class, MRB_TT_DATA);

  mrb_define_method(mrb, font_class, "initialize",  font_initialize,  MRB_ARGS_REQ(2));
  mrb_define_method(mrb, font_class, "render",      font_render,      MRB_ARGS_ARG(4,2));
  mrb_define_method(mrb, font_class, "size",        font_size,        MRB_ARGS_NONE());
  mrb_define_method(mrb, font_class, "calc_bounds", font_calc_bounds, MRB_ARGS_REQ(1));

  id_outline       = mrb_intern_cstr(mrb, "outline");
  id_outline_color = mrb_intern_cstr(mrb, "outline_color");
  id_transform     = mrb_intern_cstr(mrb, "transform");
}
