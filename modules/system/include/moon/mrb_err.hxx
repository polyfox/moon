#ifndef MOON_MRB_ERR_H
#define MOON_MRB_ERR_H

#include <stdbool.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/value.h>

extern bool mmrb_check_class(mrb_state *mrb, mrb_value obj, struct RClass *klass, bool quiet);

#endif
