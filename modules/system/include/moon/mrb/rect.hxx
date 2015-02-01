#ifndef MMRB_RECT_H
#define MMRB_RECT_H

#include <memory.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/rect.hxx"

extern const struct mrb_data_type rect_data_type;
void mmrb_rect_init(mrb_state *mrb, struct RClass *mod);
mrb_value mmrb_rect_value(mrb_state *mrb, Moon::IntRect rect);
Moon::IntRect mmrb_to_rect(mrb_state *mrb, mrb_value obj);

#endif
