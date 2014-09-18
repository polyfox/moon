#include "mrb.hxx"
#include "font.hxx"
#include <glm/glm.hpp>
#include "shared_types.hxx"

namespace Moon
{

  static mrb_sym id_outline;
  static mrb_sym id_outline_color;
  static mrb_sym id_transform;

  static void moon_mrb_font_deallocate(mrb_state *mrb, void *p) {
    delete((Font*)p);
  };

  const struct mrb_data_type font_data_type = {
    "Font", moon_mrb_font_deallocate,
  };

  static mrb_value
  moon_mrb_font_initialize(mrb_state *mrb, mrb_value self) {
    char* filename;
    mrb_int font_size;
    mrb_get_args(mrb, "zi", &filename, &font_size);

    Font *font = new Font(filename, font_size);

    DATA_TYPE(self) = &font_data_type;
    DATA_PTR(self) = font;

    return mrb_nil_value();
  };

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
  moon_mrb_font_render(mrb_state *mrb, mrb_value self) {
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
                                 &mrb_Transform::data_type, mat4_ptr);
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
        font->draw_text(x, y, z, text, **text_color); //text_color needs to be dereferenced to shared_ptr first and then to value
      } else {
        font->draw_text(x, y, z, text);
      }
    }
    delete[] text;

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_font_size(mrb_state *mrb, mrb_value self) {
    Font *font;
    Data_Get_Struct(mrb, self, &font_data_type, font);
    return mrb_fixnum_value(font->size());
  }

  static mrb_value
  moon_mrb_font_calc_bounds(mrb_state *mrb, mrb_value self) {
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
  moon_mrb_font_init(mrb_state *mrb) {
    struct RClass *font_class;
    font_class = mrb_define_class_under(mrb, moon_module, "Font", mrb->object_class);
    MRB_SET_INSTANCE_TT(font_class, MRB_TT_DATA);

    mrb_define_method(mrb, font_class, "initialize",  moon_mrb_font_initialize,  MRB_ARGS_REQ(2));
    mrb_define_method(mrb, font_class, "render",      moon_mrb_font_render,      MRB_ARGS_ARG(4,2));
    mrb_define_method(mrb, font_class, "size",        moon_mrb_font_size,        MRB_ARGS_NONE());
    mrb_define_method(mrb, font_class, "calc_bounds", moon_mrb_font_calc_bounds, MRB_ARGS_REQ(1));

    id_outline       = mrb_intern_cstr(mrb, "outline");
    id_outline_color = mrb_intern_cstr(mrb, "outline_color");
    id_transform     = mrb_intern_cstr(mrb, "transform");

    return font_class;
  }
}
