#ifndef MMRB_TRANSFORM_H
#define MMRB_TRANSFORM_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>

extern struct mrb_data_type transform_data_type;
void mmrb_transform_init(mrb_state *mrb, struct RClass *mod);

#endif
