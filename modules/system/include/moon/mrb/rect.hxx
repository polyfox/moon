#ifndef MMRB_RECT_H
#define MMRB_RECT_H

#include <memory.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/rect.hxx"
#include "moon/mrb/helpers.hxx"

MOON_C_API const struct mrb_data_type rect_data_type;
MOON_C_API void mmrb_rect_init(mrb_state *mrb);
MOON_C_API mrb_value mmrb_rect_value(mrb_state *mrb, Moon::IntRect rect);
MOON_C_API Moon::IntRect mmrb_to_rect(mrb_state *mrb, mrb_value obj);
MOON_C_API Moon::FloatRect mmrb_to_float_rect(mrb_state *mrb, mrb_value obj);

static inline struct RClass*
mmrb_get_rect_class(mrb_state *mrb)
{
  return MOON_GET_CLASS("Rect");
}

static inline Moon::IntRect*
mmrb_rect_ptr(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::IntRect*>(mrb_data_get_ptr(mrb, self, &rect_data_type));
}

#endif
