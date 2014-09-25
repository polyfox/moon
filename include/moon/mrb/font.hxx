#ifndef MMRB_FONT_H
#define MMRB_FONT_H

#include "moon/mrb.hxx"

extern struct mrb_data_type font_data_type;
extern struct RClass* mmrb_Font;
struct RClass* mmrb_font_init(mrb_state *mrb);

#endif
