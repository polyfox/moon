#include "mrb.hxx"
#include "window.hxx"

namespace Moon 
{
  /* Where the glfw window is currently, is a bit complicated to access */

  static mrb_value moon_mrb_window_get_width(mrb_state *mrb, mrb_value self) {
    return mrb_fixnum_value(640);
  }

  static mrb_value moon_mrb_window_get_height(mrb_state *mrb, mrb_value self) {
    return mrb_fixnum_value(480);
  }

  static mrb_value moon_mrb_window_uptime(mrb_state *mrb, mrb_value self) {
    return mrb_float_value(mrb, glfwGetTime());
  }

  void moon_mrb_window_init(mrb_state *mrb) {
    struct RClass *window_class;
    window_class = mrb_define_class(mrb, "Window", mrb->object_class);
    mrb_define_class_method(mrb, window_class, "width", moon_mrb_window_get_width, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, window_class, "height", moon_mrb_window_get_height, MRB_ARGS_NONE());
    mrb_define_class_method(mrb, window_class, "uptime", moon_mrb_window_uptime, MRB_ARGS_NONE());
  }
}