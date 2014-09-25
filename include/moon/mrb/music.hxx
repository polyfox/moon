#ifndef MMRB_MUSIC_H
#define MMRB_MUSIC_H

#include "moon/mrb.hxx"

extern struct mrb_data_type music_data_type;
extern struct RClass* mmrb_Music;
struct RClass* mmrb_music_init(mrb_state *mrb);

#endif
