#include "moon/mrb/font.hxx"
#include "moon/mrb/transform.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include <glm/glm.hpp>
#include "moon/font.hxx"

using Moon::Font;
using Moon::font_render_options;

static mrb_sym id_outline;
static mrb_sym id_outline_color;
static mrb_sym id_transform;

static void
font_free(mrb_state *mrb, void *p)
{
  Font *font = (Font*)p;
  if (font) {
    delete(font);
  }
}

struct mrb_data_type font_data_type = { "Font", font_free };

static mrb_value
font_initialize(mrb_state *mrb, mrb_value self)
{
  char* filename;
  mrb_int font_size;
  Font *font;
  mrb_get_args(mrb, "zi", &filename, &font_size);

  font = (Font*)DATA_PTR(self);
  if (font) {
    font_free(mrb, (void*)font);
  }

  font = new Font(filename, font_size);

  DATA_TYPE(self) = &font_data_type;
  DATA_PTR(self) = font;

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

  Font *font;
  Data_Get_Struct(mrb, self, &font_data_type, font);

  if (!mrb_nil_p(options)) {
    font_render_options render_op;

    mrb_value keys = mrb_hash_keys(mrb, options);
    int len = mrb_ary_len(mrb, keys);
    const mrb_value *keys_ary = RARRAY_PTR(keys);

    for (int i=0; i < len; ++i) {
      mrb_value key = keys_ary[i];

      if (mrb_symbol_p(key)) {
        // :opacity
        if (mrb_symbol(key) == id_outline) {
          render_op.outline = mrb_to_flo(mrb, mrb_hash_get(mrb, options, key));

        } else if (mrb_symbol(key) == id_outline_color) {
          moon_vec4* color_ptr;
          Data_Get_Struct(mrb, mrb_hash_get(mrb, options, key),
                               &vector4_data_type, color_ptr);
          render_op.outline_color = **color_ptr;
        } else if (mrb_symbol(key) == id_transform) {
          moon_mat4* mat4_ptr;
          Data_Get_Struct(mrb, mrb_hash_get(mrb, options, key),
                               &transform_data_type, mat4_ptr);
          render_op.transform = **mat4_ptr;
        }
      }
    }
    if(!mrb_nil_p(color)) {
      moon_vec4* text_color;
      Data_Get_Struct(mrb, color, &vector4_data_type, text_color);
      render_op.color = **text_color;
    }
    font->draw_text(x, y, z, text, render_op);
  } else {
    if(!mrb_nil_p(color)) {
      moon_vec4* text_color;
      Data_Get_Struct(mrb, color, &vector4_data_type, text_color);
      //text_color needs to be dereferenced to shared_ptr first and then to value
      font->draw_text(x, y, z, text, **text_color);
    } else {
      font->draw_text(x, y, z, text);
    }
  }
  delete[] text;

  return mrb_nil_value();
}

static mrb_value
font_size(mrb_state *mrb, mrb_value self)
{
  Font *font;
  Data_Get_Struct(mrb, self, &font_data_type, font);
  return mrb_fixnum_value(font->size());
}

static mrb_value
font_calc_bounds(mrb_state *mrb, mrb_value self)
{
  char* str;
  mrb_get_args(mrb, "z", &str);

  // convert to wide char (UTF-8)
  wchar_t *text = char_to_utf8(str);

  Font* font;
  Data_Get_Struct(mrb, self, &font_data_type, font);
  glm::vec2 bounds = font->compute_string_bbox(text);
  delete[] text;
  mrb_value argv[2] = { mrb_fixnum_value(bounds.x), mrb_fixnum_value(bounds.y) };


  return mrb_ary_new_from_values(mrb, 2, argv);
}

struct RClass*
mmrb_font_init(mrb_state *mrb)
{
  struct RClass *font_class;
  font_class = mrb_define_class_under(mrb, mmrb_Moon, "Font", mrb->object_class);
  MRB_SET_INSTANCE_TT(font_class, MRB_TT_DATA);

  mrb_define_method(mrb, font_class, "initialize",  font_initialize,  MRB_ARGS_REQ(2));
  mrb_define_method(mrb, font_class, "render",      font_render,      MRB_ARGS_ARG(4,2));
  mrb_define_method(mrb, font_class, "size",        font_size,        MRB_ARGS_NONE());
  mrb_define_method(mrb, font_class, "calc_bounds", font_calc_bounds, MRB_ARGS_REQ(1));

  id_outline       = mrb_intern_cstr(mrb, "outline");
  id_outline_color = mrb_intern_cstr(mrb, "outline_color");
  id_transform     = mrb_intern_cstr(mrb, "transform");

  return font_class;
}
