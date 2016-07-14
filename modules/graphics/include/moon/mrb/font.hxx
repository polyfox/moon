#ifndef MMRB_FONT_H
#define MMRB_FONT_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/font.hxx"

MOON_C_API const struct mrb_data_type font_data_type;
MOON_C_API void mmrb_font_init(mrb_state *mrb);

static inline Moon::Font*
mmrb_font_ptr(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Font*>(mrb_data_get_ptr(mrb, self, &font_data_type));
}

#endif
