#ifndef MOON_MRB_ERR_H
#define MOON_MRB_ERR_H

#include "moon/mrb.hxx"

extern bool mmrb_check_class(mrb_state *mrb, mrb_value obj, struct RClass *klass, bool quiet);

#endif
