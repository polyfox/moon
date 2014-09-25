#ifndef MMRB_CONTEXT_H
#define MMRB_CONTEXT_H

#include "moon/mrb.hxx"

extern struct mrb_data_type context_data_type;
extern struct RClass* mmrb_Context;
struct RClass* mmrb_context_init(mrb_state *mrb);

#endif
