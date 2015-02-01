#include "moon/mrb/screen.hxx"
#include "moon/mrb/window.hxx"
#include "moon/window.hxx"
#include "moon/vector4.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/shared_types.hxx"

using Moon::Window;

/* Where the glfw screen is currently, is a bit complicated to access */
static mrb_value
screen_get_width(mrb_state *mrb, mrb_value self)
{
  Window *window;

  mrb_value window_val = mrb_mod_cv_get(mrb, mmrb_Moon, mrb_intern_cstr(mrb, "window"));
  window = (Window*)mrb_data_get_ptr(mrb, window_val, &window_data_type);
  return mrb_fixnum_value(window->width());
}

static mrb_value
screen_get_height(mrb_state *mrb, mrb_value self)
{
  Window *window;

  mrb_value window_val = mrb_mod_cv_get(mrb, mmrb_Moon, mrb_intern_cstr(mrb, "window"));
  window = (Window*)mrb_data_get_ptr(mrb, window_val, &window_data_type);
  return mrb_fixnum_value(window->height());
}

static mrb_value
screen_resize(mrb_state *mrb, mrb_value self)
{
  mrb_int w, h;
  mrb_get_args(mrb, "ii", &w, &h);

  Window *window;

  mrb_value window_val = mrb_mod_cv_get(mrb, mmrb_Moon, mrb_intern_cstr(mrb, "window"));
  window = (Window*)mrb_data_get_ptr(mrb, window_val, &window_data_type);

  window->resize(w, h);

  return mrb_nil_value();
}

static mrb_value
screen_uptime(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, glfwGetTime());
}

static mrb_value
screen_get_clear_color(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, "clear_color"));
}

static mrb_value
screen_set_clear_color(mrb_state *mrb, mrb_value self)
{
  mrb_value clear_color;
  moon_vec4 *clear_color_vec;
  glm::vec4 c;
  mrb_get_args(mrb, "o", &clear_color);
  clear_color_vec = (moon_vec4*)mrb_data_get_ptr(mrb, clear_color, &vector4_data_type);
  c = moon_vec4_ref(clear_color_vec);
  glClearColor(c.r, c.g, c.b, c.a);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "clear_color"), clear_color);
  return mrb_nil_value();
}

struct RClass*
mmrb_screen_init(mrb_state *mrb)
{
  struct RClass *screen_class;
  screen_class = mrb_define_class_under(mrb, mmrb_Moon, "Screen", mrb->object_class);
  mrb_define_class_method(mrb, screen_class, "width",        screen_get_width,       MRB_ARGS_NONE());
  mrb_define_class_method(mrb, screen_class, "height",       screen_get_height,      MRB_ARGS_NONE());
  mrb_define_class_method(mrb, screen_class, "resize",       screen_resize,          MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, screen_class, "clear_color",  screen_get_clear_color, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, screen_class, "clear_color=", screen_set_clear_color, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, screen_class, "uptime",       screen_uptime,          MRB_ARGS_NONE());

  return screen_class;
}
