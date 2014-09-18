#include "moon/mrb.hxx"
#include <memory>
#include "moon/graphics.hxx"

namespace Moon {

  static void moon_mrb_rect_deallocate(mrb_state *mrb, void *p) {
    delete((std::shared_ptr<Rect>*)p);
  };

  const struct mrb_data_type rect_data_type = {
    "Rect", moon_mrb_rect_deallocate,
  };

  static mrb_value
  moon_mrb_rect_initialize(mrb_state *mrb, mrb_value self) {
    mrb_int x, y, w, h;
    mrb_get_args(mrb, "iiii", &x, &y, &w, &h);

    auto rect = new std::shared_ptr<Rect>(new Rect(x, y, w, h));

    DATA_TYPE(self) = &rect_data_type;
    DATA_PTR(self) = rect;

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_rect_initialize_copy(mrb_state *mrb, mrb_value self) {
    mrb_value other;
    std::shared_ptr<Rect>* src_rect;

    mrb_get_args(mrb, "o", &other);

    Data_Get_Struct(mrb, other, &rect_data_type, src_rect);

    auto rect = new std::shared_ptr<Rect>(new Rect((*src_rect)->x, (*src_rect)->y, (*src_rect)->w, (*src_rect)->h));
    DATA_TYPE(self) = &rect_data_type;
    DATA_PTR(self) = rect;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_rect_x_setter(mrb_state *mrb, mrb_value self) {
    mrb_int x;
    mrb_get_args(mrb, "i", &x);

    std::shared_ptr<Rect>* rect;
    Data_Get_Struct(mrb, self, &rect_data_type, rect);

    (*rect)->x = x;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_rect_x_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Rect>* rect;
    Data_Get_Struct(mrb, self, &rect_data_type, rect);

    //return mrb_fixnum_value((int)(*rect)->x);
    return mrb_float_value(mrb, (*rect)->x);
  }

  static mrb_value
  moon_mrb_rect_y_setter(mrb_state *mrb, mrb_value self) {
    mrb_int y;
    mrb_get_args(mrb, "i", &y);

    std::shared_ptr<Rect>* rect;
    Data_Get_Struct(mrb, self, &rect_data_type, rect);

    (*rect)->y = y;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_rect_y_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Rect>* rect;
    Data_Get_Struct(mrb, self, &rect_data_type, rect);

    return mrb_float_value(mrb, (*rect)->y);
  }

  static mrb_value
  moon_mrb_rect_width_setter(mrb_state *mrb, mrb_value self) {
    mrb_int width;
    mrb_get_args(mrb, "i", &width);

    std::shared_ptr<Rect>* rect;
    Data_Get_Struct(mrb, self, &rect_data_type, rect);

    (*rect)->w = width;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_rect_width_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Rect>* rect;
    Data_Get_Struct(mrb, self, &rect_data_type, rect);

    return mrb_float_value(mrb, (*rect)->w);
  }

  static mrb_value
  moon_mrb_rect_height_setter(mrb_state *mrb, mrb_value self) {
    mrb_int height;
    mrb_get_args(mrb, "i", &height);

    std::shared_ptr<Rect>* rect;
    Data_Get_Struct(mrb, self, &rect_data_type, rect);

    (*rect)->h = height;

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_rect_height_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Rect>* rect;
    Data_Get_Struct(mrb, self, &rect_data_type, rect);

    return mrb_float_value(mrb, (*rect)->h);
  }

  struct RClass*
  moon_mrb_rect_init(mrb_state *mrb) {
    struct RClass *rect_class;
    rect_class = mrb_define_class_under(mrb, moon_module, "Rect", mrb->object_class);
    MRB_SET_INSTANCE_TT(rect_class, MRB_TT_DATA);

    mrb_define_method(mrb, rect_class, "initialize",      moon_mrb_rect_initialize,      MRB_ARGS_REQ(4));
    mrb_define_method(mrb, rect_class, "initialize_copy", moon_mrb_rect_initialize_copy, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rect_class, "x=",              moon_mrb_rect_x_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rect_class, "x",               moon_mrb_rect_x_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, rect_class, "y=",              moon_mrb_rect_y_setter,        MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rect_class, "y",               moon_mrb_rect_y_getter,        MRB_ARGS_NONE());
    mrb_define_method(mrb, rect_class, "width=",          moon_mrb_rect_width_setter,    MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rect_class, "width",           moon_mrb_rect_width_getter,    MRB_ARGS_NONE());
    mrb_define_method(mrb, rect_class, "height=",         moon_mrb_rect_height_setter,   MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rect_class, "height",          moon_mrb_rect_height_getter,   MRB_ARGS_NONE());

    return rect_class;
  };

}
