#ifndef MMRB_RECT_H
#define MMRB_RECT_H

#include <memory>
#include "moon/mrb.hxx"
#include "moon/rect.hxx"

extern struct mrb_data_type rect_data_type;
extern struct RClass* mmrb_Rect;
struct RClass* mmrb_rect_init(mrb_state *mrb);

mrb_value mmrb_rect_wrap(mrb_state *mrb, Moon::Rect *ptr);
mrb_value mmrb_rect_create(mrb_state *mrb, GLint x, GLint y, GLint w, GLint h);

#endif
