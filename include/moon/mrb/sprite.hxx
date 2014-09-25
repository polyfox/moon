#ifndef MMRB_SPRITE_H
#define MMRB_SPRITE_H

#include "moon/mrb.hxx"

extern struct mrb_data_type sprite_data_type;
extern struct RClass* mmrb_Sprite;
struct RClass* mmrb_sprite_init(mrb_state *mrb);

#endif
