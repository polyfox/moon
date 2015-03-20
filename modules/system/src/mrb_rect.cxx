#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/object.h>
#include <mruby/value.h>
#include "moon/mrb/rect.hxx"
#include "moon/rect.hxx"

static struct RClass *rect_class;

static void
rect_free(mrb_state *mrb, void *p)
{
  Moon::IntRect *rect = (Moon::IntRect*)p;
  if (rect) {
    delete(rect);
  }
}

MOON_C_API const struct mrb_data_type rect_data_type = { "Moon::IntRect", rect_free };

static inline Moon::IntRect*
get_rect(mrb_state *mrb, mrb_value self)
{
  return (Moon::IntRect*)mrb_data_get_ptr(mrb, self, &rect_data_type);
}

static inline void
cleanup_rect(mrb_state *mrb, mrb_value self)
{
  void *ptr = DATA_PTR(self);
  if (ptr) {
    rect_free(mrb, ptr);
  }
}

MOON_C_API Moon::IntRect
mmrb_to_rect(mrb_state *mrb, mrb_value self)
{
  const mrb_vtype type = mrb_type(self);
  if (type == MRB_TT_DATA) {
    const mrb_data_type *dt = DATA_TYPE(self);
    if (dt == &rect_data_type) {
      return *(Moon::IntRect*)DATA_PTR(self);
    }
  } else if (type == MRB_TT_ARRAY) {
    //
  } else if (type == MRB_TT_FIXNUM) {
    //
  }
  mrb_raise(mrb, E_TYPE_ERROR, "unexpected type");
  return Moon::IntRect{0, 0, 0, 0};
}

MOON_C_API mrb_value
mmrb_rect_value(mrb_state *mrb, Moon::IntRect rect)
{
  Moon::IntRect *target;
  mrb_value result;
  result = mrb_obj_new(mrb, rect_class, 0, NULL);
  target = get_rect(mrb, result);
  *target = rect;
  return result;
}

static mrb_value
rect_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y, w, h;
  Moon::IntRect *rect;
  mrb_get_args(mrb, "iiii", &x, &y, &w, &h);
  cleanup_rect(mrb, self);
  rect = new Moon::IntRect(x, y, w, h);
  mrb_data_init(self, rect, &rect_data_type);
  return self;
}

static mrb_value
rect_initialize_copy(mrb_state *mrb, mrb_value self)
{
  Moon::IntRect *other;
  mrb_get_args(mrb, "d", &other, &rect_data_type);
  cleanup_rect(mrb, self);
  mrb_data_init(self, new Moon::IntRect(*other), &rect_data_type);
  return self;
}

static mrb_value
rect_set_x(mrb_state *mrb, mrb_value self)
{
  mrb_int x;
  mrb_get_args(mrb, "i", &x);
  get_rect(mrb, self)->x = x;
  return mrb_nil_value();
}

static mrb_value
rect_get_x(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_rect(mrb, self)->x);
}

static mrb_value
rect_set_y(mrb_state *mrb, mrb_value self)
{
  mrb_int y;
  mrb_get_args(mrb, "i", &y);
  get_rect(mrb, self)->y = y;
  return mrb_nil_value();
}

static mrb_value
rect_get_y(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_rect(mrb, self)->y);
}

/*
 * Rect#width=
 # @param [Integer] width
 */
static mrb_value
rect_set_width(mrb_state *mrb, mrb_value self)
{
  mrb_int w;
  mrb_get_args(mrb, "i", &w);
  get_rect(mrb, self)->w = w;
  return self;
}

/*
 * Rect#width
 # @return [Integer]
 */
static mrb_value
rect_get_width(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_rect(mrb, self)->w);
}

/*
 * Rect#height=
 # @param [Integer] height
 */
static mrb_value
rect_set_height(mrb_state *mrb, mrb_value self)
{
  mrb_int h;
  mrb_get_args(mrb, "i", &h);
  get_rect(mrb, self)->h = h;
  return self;
}

/*
 * Rect#height
 # @return [Integer]
 */
static mrb_value
rect_get_height(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_rect(mrb, self)->h);
}

MOON_C_API void
mmrb_rect_init(mrb_state *mrb, struct RClass *mod)
{
  rect_class = mrb_define_class_under(mrb, mod, "Rect", mrb->object_class);
  MRB_SET_INSTANCE_TT(rect_class, MRB_TT_DATA);

  mrb_define_method(mrb, rect_class, "initialize",      rect_initialize,      MRB_ARGS_REQ(4));
  mrb_define_method(mrb, rect_class, "initialize_copy", rect_initialize_copy, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "x=",              rect_set_x,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "x",               rect_get_x,        MRB_ARGS_NONE());
  mrb_define_method(mrb, rect_class, "y=",              rect_set_y,        MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "y",               rect_get_y,        MRB_ARGS_NONE());
  mrb_define_method(mrb, rect_class, "width=",          rect_set_width,    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "width",           rect_get_width,    MRB_ARGS_NONE());
  mrb_define_method(mrb, rect_class, "height=",         rect_set_height,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "height",          rect_get_height,   MRB_ARGS_NONE());
}
