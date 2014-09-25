#ifndef MMRB_SPRITESHEET_H
#define MMRB_SPRITESHEET_H

#include "moon/mrb.hxx"

extern struct mrb_data_type spritesheet_data_type;
extern struct RClass* mmrb_Spritesheet;
struct RClass* mmrb_spritesheet_init(mrb_state *mrb);

#endif
