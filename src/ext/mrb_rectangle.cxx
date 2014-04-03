#include "mrb.hxx"
#include <memory>
#include "graphics.hxx"

namespace Moon {

  static void moon_mrb_rectangle_deallocate(mrb_state *mrb, void *p) {
    delete((std::shared_ptr<Rect>*)p);
  };

  const struct mrb_data_type rectangle_data_type = {
    "Rect", moon_mrb_rectangle_deallocate,
  };

  static mrb_value moon_mrb_rectangle_new(mrb_state *mrb, mrb_value klass) {
    mrb_int x, y, w, h;
    mrb_get_args(mrb, "iiii", &x, &y, &w, &h);

    auto rectangle = new std::shared_ptr<Rect>(new Rect(x, y, w, h));

    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_ptr(klass), &rectangle_data_type, rectangle));
  };

  static mrb_value moon_mrb_rectangle_x_setter(mrb_state *mrb, mrb_value self) {
    mrb_int x;
    mrb_get_args(mrb, "i", &x);

    std::shared_ptr<Rect>* rectangle;
    Data_Get_Struct(mrb, self, &rectangle_data_type, rectangle);

    (*rectangle)->x = x;

    return mrb_float_value(mrb, (*rectangle)->x);
  }

  static mrb_value moon_mrb_rectangle_x_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Rect>* rectangle;
    Data_Get_Struct(mrb, self, &rectangle_data_type, rectangle);

    return mrb_float_value(mrb, (*rectangle)->x);
  }

  static mrb_value moon_mrb_rectangle_y_setter(mrb_state *mrb, mrb_value self) {
    mrb_int y;
    mrb_get_args(mrb, "i", &y);

    std::shared_ptr<Rect>* rectangle;
    Data_Get_Struct(mrb, self, &rectangle_data_type, rectangle);

    (*rectangle)->y = y;

    return mrb_float_value(mrb, (*rectangle)->y);
  }

  static mrb_value moon_mrb_rectangle_y_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Rect>* rectangle;
    Data_Get_Struct(mrb, self, &rectangle_data_type, rectangle);

    return mrb_float_value(mrb, (*rectangle)->y);
  }

  static mrb_value moon_mrb_rectangle_width_setter(mrb_state *mrb, mrb_value self) {
    mrb_int width;
    mrb_get_args(mrb, "i", &width);

    std::shared_ptr<Rect>* rectangle;
    Data_Get_Struct(mrb, self, &rectangle_data_type, rectangle);

    (*rectangle)->w = width;

    return mrb_float_value(mrb, (*rectangle)->w);
  }

  static mrb_value moon_mrb_rectangle_width_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Rect>* rectangle;
    Data_Get_Struct(mrb, self, &rectangle_data_type, rectangle);

    return mrb_float_value(mrb, (*rectangle)->w);
  }

  static mrb_value moon_mrb_rectangle_height_setter(mrb_state *mrb, mrb_value self) {
    mrb_int height;
    mrb_get_args(mrb, "i", &height);

    std::shared_ptr<Rect>* rectangle;
    Data_Get_Struct(mrb, self, &rectangle_data_type, rectangle);

    (*rectangle)->h = height;

    return mrb_float_value(mrb, (*rectangle)->h);
  }

  static mrb_value moon_mrb_rectangle_height_getter(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Rect>* rectangle;
    Data_Get_Struct(mrb, self, &rectangle_data_type, rectangle);

    return mrb_float_value(mrb, (*rectangle)->h);
  }

  struct RClass* moon_mrb_rectangle_init(mrb_state *mrb) {
    struct RClass *rectangle_class;
    rectangle_class = mrb_define_class_under(mrb, mrb_module_get(mrb, "Moon"), "Rect", mrb->object_class);
    MRB_SET_INSTANCE_TT(rectangle_class, MRB_TT_DATA);

    mrb_define_class_method(mrb, rectangle_class, "new", moon_mrb_rectangle_new,            MRB_ARGS_REQ(3));
    mrb_define_method(mrb, rectangle_class, "x=",        moon_mrb_rectangle_x_setter,       MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rectangle_class, "x",         moon_mrb_rectangle_x_getter,       MRB_ARGS_NONE());
    mrb_define_method(mrb, rectangle_class, "y=",        moon_mrb_rectangle_y_setter,       MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rectangle_class, "y",         moon_mrb_rectangle_y_getter,       MRB_ARGS_NONE());
    mrb_define_method(mrb, rectangle_class, "width=",    moon_mrb_rectangle_width_setter,   MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rectangle_class, "width",     moon_mrb_rectangle_width_getter,   MRB_ARGS_NONE());
    mrb_define_method(mrb, rectangle_class, "height=",   moon_mrb_rectangle_height_setter,  MRB_ARGS_REQ(1));
    mrb_define_method(mrb, rectangle_class, "height",    moon_mrb_rectangle_height_getter,  MRB_ARGS_NONE());

    return rectangle_class;
  };

}