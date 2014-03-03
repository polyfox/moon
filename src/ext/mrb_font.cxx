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

  static mrb_value moon_mrb_font_new(mrb_state *mrb, mrb_value klass) {
    char* filename;
    mrb_int font_size;
    mrb_get_args(mrb, "zi", &filename, &font_size);

    Font *font = new Font(filename, font_size);
    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_ptr(klass), &font_data_type, font));
  };

  static mrb_value moon_mrb_font_draw_text(mrb_state *mrb, mrb_value self) {
    mrb_float x, y;
    mrb_value color;
    char* str;
    mrb_get_args(mrb, "ffz|o", &x, &y, &str, &color);

    // convert to wide char (UTF-8)
    wchar_t *text = char_to_utf8(str);

    Font *font;
    Data_Get_Struct(mrb, self, &font_data_type, font);

    if(!mrb_nil_p(color)) {
      std::shared_ptr<Color>* text_color;
      Data_Get_Struct(mrb, color, &color_data_type, text_color);
      font->draw_text(x, y, text, **text_color); //text_color needs to be dereferenced to shared_ptr first and then to value
    } else {
      font->draw_text(x, y, text);
    }
    delete[] text;

    return mrb_nil_value();
  };

  static mrb_value moon_mrb_font_size(mrb_state *mrb, mrb_value self) {
    Font *font;
    Data_Get_Struct(mrb, self, &font_data_type, font);
    return mrb_fixnum_value(font->size());
  }

  struct RClass* moon_mrb_font_init(mrb_state *mrb) {
    struct RClass *font_class;
    font_class = mrb_define_class_under(mrb, mrb_module_get(mrb, "Moon"), "Font", mrb->object_class);

    mrb_define_class_method(mrb, font_class, "new", moon_mrb_font_new, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, font_class, "draw_text", moon_mrb_font_draw_text, MRB_ARGS_REQ(3));
    mrb_define_method(mrb, font_class, "size",      moon_mrb_font_size, MRB_ARGS_NONE());

    return font_class;
  }
}