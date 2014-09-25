#ifndef MMRB_SOUND_H
#define MMRB_SOUND_H

#include "moon/mrb.hxx"

extern struct mrb_data_type sound_data_type;
extern struct RClass* mmrb_Sound;
struct RClass* mmrb_sound_init(mrb_state *mrb);

#endif
