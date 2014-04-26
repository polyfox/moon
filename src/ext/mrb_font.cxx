#include "mrb.hxx"
#include "font.hxx"

namespace Moon
{
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
   * Font#draw_text(x, y, str[, color])
   * @param [Numeric] x
   * @param [Numeric] y
   * @param [String] str
   * @param [Color] color
   *   @optional
   */
  static mrb_value
  moon_mrb_font_render(mrb_state *mrb, mrb_value self) {
    mrb_float x, y, z;
    mrb_value color = mrb_nil_value();
    char* str;
    mrb_get_args(mrb, "fffz|o", &x, &y, &z, &str, &color);

    // convert to wide char (UTF-8)
    wchar_t *text = char_to_utf8(str);

    Font *font;
    Data_Get_Struct(mrb, self, &font_data_type, font);

    if(!mrb_nil_p(color)) {
      std::shared_ptr<Color>* text_color;
      Data_Get_Struct(mrb, color, &color_data_type, text_color);
      font->draw_text(x, y, z, text, **text_color); //text_color needs to be dereferenced to shared_ptr first and then to value
    } else {
      font->draw_text(x, y, z, text);
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
    glm::vec2 bounds = font->calc_bounds(text);
    delete[] text;
    mrb_value argv[2] = { mrb_float_value(mrb, bounds.x), mrb_float_value(mrb, bounds.y) };


    return mrb_ary_new_from_values(mrb, 2, argv);
  }

  struct RClass*
  moon_mrb_font_init(mrb_state *mrb) {
    struct RClass *font_class;
    font_class = mrb_define_class_under(mrb, moon_module, "Font", mrb->object_class);
    MRB_SET_INSTANCE_TT(font_class, MRB_TT_DATA);

    mrb_define_method(mrb, font_class, "initialize",  moon_mrb_font_initialize,  MRB_ARGS_REQ(2));
    mrb_define_method(mrb, font_class, "render",      moon_mrb_font_render,      MRB_ARGS_ARG(4,1));
    mrb_define_method(mrb, font_class, "size",        moon_mrb_font_size,        MRB_ARGS_NONE());
    mrb_define_method(mrb, font_class, "calc_bounds", moon_mrb_font_calc_bounds, MRB_ARGS_REQ(1));

    return font_class;
  }
}