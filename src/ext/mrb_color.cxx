#include "mrb.hxx"
#include "graphics.hxx"
#include <memory>

namespace Moon {

  static void moon_mrb_color_deallocate(mrb_state *mrb, void *p) {
    delete((std::shared_ptr<Color>*)p);
  };

  const struct mrb_data_type color_data_type = {
    "Color", moon_mrb_color_deallocate,
  };

  static mrb_value moon_mrb_color_new(mrb_state *mrb, mrb_value klass) {
    mrb_float r, g, b, a;
    mrb_get_args(mrb, "ffff", &r, &g, &b, &a);

    auto color = new std::shared_ptr<Color>(new Color(r, g, b, a));

    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_ptr(klass), &color_data_type, color));
  };

  static mrb_value moon_mrb_color_red_setter(mrb_state *mrb, mrb_value self) {
    mrb_float red;
    mrb_get_args(mrb, "f", &red);

    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    (*color)->r = red;

    return mrb_float_value(mrb, (*color)->r);
  }

  static mrb_value moon_mrb_color_red_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    return mrb_float_value(mrb, (*color)->r);
  }

  static mrb_value moon_mrb_color_green_setter(mrb_state *mrb, mrb_value self) {
    mrb_float green;
    mrb_get_args(mrb, "f", &green);

    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    (*color)->g = green;

    return mrb_float_value(mrb, (*color)->g);
  }

  static mrb_value moon_mrb_color_green_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    return mrb_float_value(mrb, (*color)->g);
  }

  static mrb_value moon_mrb_color_blue_setter(mrb_state *mrb, mrb_value self) {
    mrb_float blue;
    mrb_get_args(mrb, "f", &blue);

    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    (*color)->b = blue;

    return mrb_float_value(mrb, (*color)->b);
  }

  static mrb_value moon_mrb_color_blue_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    return mrb_float_value(mrb, (*color)->b);
  }

  static mrb_value moon_mrb_color_alpha_setter(mrb_state *mrb, mrb_value self) {
    mrb_float alpha;
    mrb_get_args(mrb, "f", &alpha);

    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    (*color)->a = alpha;

    return mrb_float_value(mrb, (*color)->a);
  }

  static mrb_value moon_mrb_color_alpha_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    return mrb_float_value(mrb, (*color)->a);
  }

  void moon_mrb_color_init(mrb_state *mrb) {
    struct RClass *color_class;
    color_class = mrb_define_class_under(mrb, mrb_class_get(mrb, "Moon"), "Color", mrb->object_class);
    MRB_SET_INSTANCE_TT(color_class, MRB_TT_DATA);

    mrb_define_class_method(mrb, color_class, "new", moon_mrb_color_new, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, color_class, "red=", moon_mrb_color_red_setter, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, color_class, "red", moon_mrb_color_red_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, color_class, "green=", moon_mrb_color_green_setter, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, color_class, "green", moon_mrb_color_green_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, color_class, "blue=", moon_mrb_color_blue_setter, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, color_class, "blue", moon_mrb_color_blue_getter, MRB_ARGS_NONE());
    mrb_define_method(mrb, color_class, "alpha=", moon_mrb_color_alpha_setter, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, color_class, "alpha", moon_mrb_color_alpha_getter, MRB_ARGS_NONE()); 
  };

}