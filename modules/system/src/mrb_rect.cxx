#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/numeric.h>
#include <mruby/object.h>
#include <mruby/string.h>
#include <mruby/value.h>
#include "moon/mrb/rect.hxx"
#include "moon/rect.hxx"

static void
rect_free(mrb_state *mrb, void *p)
{
  Moon::IntRect *rect = (Moon::IntRect*)p;
  if (rect) {
    delete(rect);
  }
}

MOON_C_API const struct mrb_data_type rect_data_type = { "Moon::IntRect", rect_free };

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
  switch (type) {
    case MRB_TT_DATA: {
      Moon::IntRect *rect = static_cast<Moon::IntRect*>(mrb_data_check_get_ptr(mrb, self, &rect_data_type));
      if (rect) return *rect;
    }; break;
    case MRB_TT_ARRAY: {
      mrb_int alen = mrb_ary_len(mrb, self);
      if (alen != 4) {
        mrb_raisef(mrb, E_ARGUMENT_ERROR,
          "wrong Array size %S (expected %S)",
          mrb_fixnum_to_str(mrb, mrb_fixnum_value(alen), 10), mrb_str_new_cstr(mrb, "4"));
      } else {
        return Moon::IntRect(
          mrb_int(mrb, mrb_ary_entry(self, 0)),
          mrb_int(mrb, mrb_ary_entry(self, 1)),
          mrb_int(mrb, mrb_ary_entry(self, 2)),
          mrb_int(mrb, mrb_ary_entry(self, 3))
        );
      }
    }; break;
    case MRB_TT_FIXNUM:
    case MRB_TT_FLOAT: {
      mrb_int val = mrb_int(mrb, self);
      return Moon::IntRect(0, 0, val, val);
    }; break;
    default:
      break;
  }
  mrb_raise(mrb, E_TYPE_ERROR, "unexpected type");
  return Moon::IntRect{0, 0, 0, 0};
}

MOON_C_API Moon::FloatRect
mmrb_to_float_rect(mrb_state *mrb, mrb_value obj)
{
  const mrb_vtype type = mrb_type(obj);
  switch (type) {
    case MRB_TT_DATA: {
      Moon::IntRect *rect = static_cast<Moon::IntRect*>(mrb_data_check_get_ptr(mrb, obj, &rect_data_type));
      if (rect) return Moon::FloatRect(rect->x, rect->y, rect->w, rect->h);
    }; break;
    case MRB_TT_ARRAY: {
      mrb_int alen = mrb_ary_len(mrb, obj);
      if (alen != 4) {
        mrb_raisef(mrb, E_ARGUMENT_ERROR,
          "wrong Array size %S (expected %S)",
          mrb_fixnum_to_str(mrb, mrb_fixnum_value(alen), 10), mrb_str_new_cstr(mrb, "4"));
      } else {
        return Moon::FloatRect(
          mrb_to_flo(mrb, mrb_ary_entry(obj, 0)),
          mrb_to_flo(mrb, mrb_ary_entry(obj, 1)),
          mrb_to_flo(mrb, mrb_ary_entry(obj, 2)),
          mrb_to_flo(mrb, mrb_ary_entry(obj, 3))
        );
      }
    }; break;
    case MRB_TT_FIXNUM:
    case MRB_TT_FLOAT: {
      mrb_float val = mrb_to_flo(mrb, obj);
      return Moon::FloatRect(0, 0, val, val);
    }; break;
    default:
      break;
  }
  mrb_raise(mrb, E_TYPE_ERROR, "unexpected type");
  return Moon::FloatRect{0, 0, 0, 0};
}

MOON_C_API mrb_value
mmrb_rect_value(mrb_state *mrb, Moon::IntRect rect)
{
  Moon::IntRect *target;
  mrb_value result;
  result = mrb_obj_new(mrb, mmrb_get_rect_class(mrb), 0, NULL);
  target = mmrb_rect_ptr(mrb, result);
  *target = rect;
  return result;
}

/*
 * @param [Integer] x
 * @param [Integer] y
 * @param [Integer] w
 * @param [Integer] h
 * @return [Rect]
 */
static mrb_value
rect_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int x = 0, y = 0, w = 0, h = 0;
  Moon::IntRect *rect;
  mrb_get_args(mrb, "|iiii", &x, &y, &w, &h);
  cleanup_rect(mrb, self);
  rect = new Moon::IntRect(x, y, w, h);
  mrb_data_init(self, rect, &rect_data_type);
  return self;
}

/**
 * Initializes the rect's parameters given another rect
 *
 * @param [Moon::Rect] other
 * @return [self]
 */
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
  mmrb_rect_ptr(mrb, self)->x = x;
  return mrb_nil_value();
}

static mrb_value
rect_get_x(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mmrb_rect_ptr(mrb, self)->x);
}

static mrb_value
rect_set_y(mrb_state *mrb, mrb_value self)
{
  mrb_int y;
  mrb_get_args(mrb, "i", &y);
  mmrb_rect_ptr(mrb, self)->y = y;
  return mrb_nil_value();
}

static mrb_value
rect_get_y(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mmrb_rect_ptr(mrb, self)->y);
}

static mrb_value
rect_set_w(mrb_state *mrb, mrb_value self)
{
  mrb_int w;
  mrb_get_args(mrb, "i", &w);
  mmrb_rect_ptr(mrb, self)->w = w;
  return self;
}

static mrb_value
rect_get_w(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mmrb_rect_ptr(mrb, self)->w);
}

static mrb_value
rect_set_h(mrb_state *mrb, mrb_value self)
{
  mrb_int h;
  mrb_get_args(mrb, "i", &h);
  mmrb_rect_ptr(mrb, self)->h = h;
  return self;
}

static mrb_value
rect_get_h(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(mmrb_rect_ptr(mrb, self)->h);
}

MOON_C_API void
mmrb_rect_init(mrb_state *mrb)
{
  struct RClass *mod = mrb_define_module(mrb, "Moon");
  struct RClass *rect_class = mrb_define_class_under(mrb, mod, "Rect", mrb->object_class);
  MRB_SET_INSTANCE_TT(rect_class, MRB_TT_DATA);

  mrb_define_method(mrb, rect_class, "initialize",      rect_initialize,      MRB_ARGS_ARG(0,4));
  mrb_define_method(mrb, rect_class, "initialize_copy", rect_initialize_copy, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "x=",              rect_set_x,           MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "x",               rect_get_x,           MRB_ARGS_NONE());
  mrb_define_method(mrb, rect_class, "y=",              rect_set_y,           MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "y",               rect_get_y,           MRB_ARGS_NONE());
  mrb_define_method(mrb, rect_class, "w=",              rect_set_w,           MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "w",               rect_get_w,           MRB_ARGS_NONE());
  mrb_define_method(mrb, rect_class, "h=",              rect_set_h,           MRB_ARGS_REQ(1));
  mrb_define_method(mrb, rect_class, "h",               rect_get_h,           MRB_ARGS_NONE());
}
