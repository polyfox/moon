#ifndef MMRB_SCREEN_H
#define MMRB_SCREEN_H

#include "moon/mrb.hxx"

//struct mrb_data_type screen_data_type;
extern struct RClass* mmrb_Screen;
struct RClass* mmrb_screen_init(mrb_state *mrb);

#endif
