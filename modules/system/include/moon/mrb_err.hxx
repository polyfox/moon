#ifndef MOON_MRB_ERR_H
#define MOON_MRB_ERR_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/value.h>
#include <stdbool.h>

extern bool mmrb_check_class(mrb_state* mrb, mrb_value obj,
                             struct RClass* klass, bool quiet);

#endif
