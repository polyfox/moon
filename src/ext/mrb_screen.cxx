#include "moon/mrb.hxx"
#include "moon/window.hxx"

namespace Moon
{
  /* Where the glfw screen is currently, is a bit complicated to access */

  static mrb_value
  moon_mrb_screen_get_width(mrb_state *mrb, mrb_value self) {
    Window *window;

    mrb_value window_val = mrb_mod_cv_get(mrb, moon_module, mrb_intern_cstr(mrb, "window"));
    Data_Get_Struct(mrb, window_val, &window_data_type, window);
    return mrb_fixnum_value(window->width());
  }

  static mrb_value
  moon_mrb_screen_get_height(mrb_state *mrb, mrb_value self) {
    Window *window;

    mrb_value window_val = mrb_mod_cv_get(mrb, moon_module, mrb_intern_cstr(mrb, "window"));
    Data_Get_Struct(mrb, window_val, &window_data_type, window);
    return mrb_fixnum_value(window->height());
  }

  static mrb_value
  moon_mrb_screen_resize(mrb_state *mrb, mrb_value self) {
    mrb_int w, h;
    mrb_get_args(mrb, "ii", &w, &h);

    Window *window;

    mrb_value window_val = mrb_mod_cv_get(mrb, moon_module, mrb_intern_cstr(mrb, "window"));
    Data_Get_Struct(mrb, window_val, &window_data_type, window);

    window->resize(w, h);

    return mrb_nil_value();
  }

  static mrb_value
  moon_mrb_screen_uptime(mrb_state *mrb, mrb_value self) {
    return mrb_float_value(mrb, glfwGetTime());
  }

  struct RClass*
  moon_mrb_screen_init(mrb_state *mrb) {
    struct RClass *screen_class;
    screen_class = mrb_define_class_under(mrb, moon_module, "Screen", mrb->object_class);
    mrb_define_class_method(mrb, screen_class, "width",  moon_mrb_screen_get_width,  MRB_ARGS_NONE());
    mrb_define_class_method(mrb, screen_class, "height", moon_mrb_screen_get_height, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, screen_class, "resize", moon_mrb_screen_resize,     MRB_ARGS_REQ(2));
    mrb_define_class_method(mrb, screen_class, "uptime", moon_mrb_screen_uptime,     MRB_ARGS_NONE());

    return screen_class;
  }
}
