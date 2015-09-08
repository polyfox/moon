#ifndef MMRB_AUDIO_STREAM_H
#define MMRB_AUDIO_STREAM_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"

MOON_C_API const struct mrb_data_type audio_stream_data_type;
MOON_C_API void mmrb_audio_stream_init(mrb_state *mrb, struct RClass *mod);

#endif
