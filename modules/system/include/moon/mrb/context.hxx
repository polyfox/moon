#ifndef MMRB_CONTEXT_H
#define MMRB_CONTEXT_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>

extern struct mrb_data_type context_data_type;
void mmrb_context_init(mrb_state *mrb, struct RClass *mod);

#endif
