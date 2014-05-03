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

  static mrb_value
  moon_mrb_color_initialize(mrb_state *mrb, mrb_value self) {
    mrb_float r, g, b, a;
    mrb_get_args(mrb, "ffff", &r, &g, &b, &a);

    r = glm::clamp(r, 0.0, 1.0);
    g = glm::clamp(g, 0.0, 1.0);
    b = glm::clamp(b, 0.0, 1.0);
    a = glm::clamp(a, 0.0, 1.0);

    auto color = new std::shared_ptr<Color>(new Color(r, g, b, a));

    DATA_TYPE(self) = &color_data_type;
    DATA_PTR(self) = color;

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_color_initialize_copy(mrb_state *mrb, mrb_value self) {
    mrb_value other;
    std::shared_ptr<Color>* src_color;

    mrb_get_args(mrb, "o", &other);

    Data_Get_Struct(mrb, other, &color_data_type, src_color);

    auto color = new std::shared_ptr<Color>(new Color((*src_color)->r, (*src_color)->g, (*src_color)->b, (*src_color)->a));
    DATA_TYPE(self) = &color_data_type;
    DATA_PTR(self) = color;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_color_red_setter(mrb_state *mrb, mrb_value self) {
    mrb_float red;
    mrb_get_args(mrb, "f", &red);

    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    (*color)->r = glm::clamp(red, 0.0, 1.0);

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_color_red_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    return mrb_float_value(mrb, (*color)->r);
  }

  static mrb_value
  moon_mrb_color_green_setter(mrb_state *mrb, mrb_value self) {
    mrb_float green;
    mrb_get_args(mrb, "f", &green);

    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    (*color)->g = glm::clamp(green, 0.0, 1.0);

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_color_green_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    return mrb_float_value(mrb, (*color)->g);
  }

  static mrb_value
  moon_mrb_color_blue_setter(mrb_state *mrb, mrb_value self) {
    mrb_float blue;
    mrb_get_args(mrb, "f", &blue);

    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    (*color)->b = glm::clamp(blue, 0.0, 1.0);

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_color_blue_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    return mrb_float_value(mrb, (*color)->b);
  }

  static mrb_value
  moon_mrb_color_alpha_setter(mrb_state *mrb, mrb_value self) {
    mrb_float alpha;
    mrb_get_args(mrb, "f", &alpha);

    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    (*color)->a = glm::clamp(alpha, 0.0, 1.0);

    return mrb_float_value(mrb, (*color)->a);
  }

  static mrb_value
  moon_mrb_color_alpha_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Color>* color;
    Data_Get_Struct(mrb, self, &color_data_type, color);

    return mrb_float_value(mrb, (*color)->a);
  }

  struct RClass*
  moon_mrb_color_init(mrb_state *mrb) {
    struct RClass *color_class;
    color_class = mrb_define_class_under(mrb, moon_module, "Color", mrb->object_class);
    MRB_SET_INSTANCE_TT(color_class, MRB_TT_DATA);

    mrb_define_method(mrb, color_class, "initialize",      moon_mrb_color_initialize,      MRB_ARGS_REQ(4));
    mrb_define_method(mrb, color_class, "initialize_copy", moon_mrb_color_initialize_copy, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, color_class, "red=",            moon_mrb_color_red_setter,      MRB_ARGS_REQ(1));
    mrb_define_method(mrb, color_class, "red",             moon_mrb_color_red_getter,      MRB_ARGS_NONE());
    mrb_define_method(mrb, color_class, "green=",          moon_mrb_color_green_setter,    MRB_ARGS_REQ(1));
    mrb_define_method(mrb, color_class, "green",           moon_mrb_color_green_getter,    MRB_ARGS_NONE());
    mrb_define_method(mrb, color_class, "blue=",           moon_mrb_color_blue_setter,     MRB_ARGS_REQ(1));
    mrb_define_method(mrb, color_class, "blue",            moon_mrb_color_blue_getter,     MRB_ARGS_NONE());
    mrb_define_method(mrb, color_class, "alpha=",          moon_mrb_color_alpha_setter,    MRB_ARGS_REQ(1));
    mrb_define_method(mrb, color_class, "alpha",           moon_mrb_color_alpha_getter,    MRB_ARGS_NONE());

    return color_class;
  };

}