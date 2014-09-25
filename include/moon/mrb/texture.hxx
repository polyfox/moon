#ifndef MMRB_TEXTURE_H
#define MMRB_TEXTURE_H

#include "moon/mrb.hxx"

extern struct mrb_data_type texture_data_type;
extern struct RClass* mmrb_Texture;
struct RClass* mmrb_texture_init(mrb_state *mrb);

#endif
