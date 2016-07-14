#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include "moon/api.h"
#include "moon/mrb/font.hxx"
#include "moon/mrb/matrix4.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/glm.h"
#include "moon/string.hxx"
#include "moon/font.hxx"
#include "moon/mrb/helpers.hxx"

static void
font_free(mrb_state *mrb, void *p)
{
  Moon::Font *font = (Moon::Font*)p;
  if (font) {
    delete(font);
  }
}

MOON_C_API const struct mrb_data_type font_data_type = { "Moon::Font", font_free };

/*
 * Calculates the string's width and height in pixel using the font.
 *
 * @param [String] filename Path to the font file
 * @param [Integer] font_size Desired font size
 */
static mrb_value
font_initialize(mrb_state *mrb, mrb_value self)
{
  char* filename;
  mrb_int font_size;
  Moon::Font *font = NULL;

  font_free(mrb, DATA_PTR(self));
  DATA_PTR(self) = NULL;

  mrb_get_args(mrb, "zi", &filename, &font_size);
  if (exists(std::string(filename))) {
    font = new Moon::Font(filename, font_size);
  } else {
    mrb_raisef(mrb, E_SCRIPT_ERROR,
                   "cannot load such file -- %S",
                   mrb_str_new_cstr(mrb, filename));
  }
  // This should never happen
  if (!font) {
    return mrb_nil_value();
  }
  mrb_data_init(self, font, &font_data_type);
  return self;
}

/*
 * Size of the font.
 * @return [Integer]
 */
static mrb_value
font_size(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mmrb_font_ptr(mrb, self)->GetSize());
}

/**
 * Calculates the string's width and height in pixel using the font.
 *
 * @param [String] str
 * @param [Float] line_height
 * @return [Array<Integer>[2]]
 */
static mrb_value
font_calc_bounds(mrb_state *mrb, mrb_value self)
{
  char *str;
  mrb_float line_height = 1.2;
  mrb_get_args(mrb, "z|f", &str, &line_height);
  // convert to wide char (UTF-8)
  Moon::String text(str);
  Moon::Font *font = mmrb_font_ptr(mrb, self);
  Moon::Vector2 bounds = font->ComputeStringBbox(text, line_height);
  mrb_value argv[2] = { mrb_fixnum_value(bounds.x), mrb_fixnum_value(bounds.y) };
  return mrb_ary_new_from_values(mrb, 2, argv);
}

/**
 * Returns the font's ascender value
 *
 * @return [Float]
 */
static mrb_value
font_ascender_get(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mmrb_font_ptr(mrb, self)->font->ascender);
}

/**
 * Binds the font's texture for drawing
 *
 * @return [self]
 */
static mrb_value
font_bind(mrb_state *mrb, mrb_value self)
{
  mmrb_font_ptr(mrb, self)->Bind();
  return self;
}

MOON_C_API void
mmrb_font_init(mrb_state *mrb)
{
  struct RClass *mod = mrb_define_module(mrb, "Moon");
  /* Class for loading and rendering font files
   * Supported formats are .ttf and possibly .otf
   */
  struct RClass *font_class = mrb_define_class_under(mrb, mod, "Font", mrb->object_class);
  MRB_SET_INSTANCE_TT(font_class, MRB_TT_DATA);

  mrb_define_method(mrb, font_class, "initialize",  font_initialize,   MRB_ARGS_REQ(2));
  mrb_define_method(mrb, font_class, "size",        font_size,         MRB_ARGS_NONE());
  mrb_define_method(mrb, font_class, "calc_bounds", font_calc_bounds,  MRB_ARGS_ARG(1,1));
  mrb_define_method(mrb, font_class, "ascender",    font_ascender_get, MRB_ARGS_NONE());
  mrb_define_method(mrb, font_class, "bind",        font_bind,         MRB_ARGS_NONE());
}
