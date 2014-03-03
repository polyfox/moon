#ifndef MOON_MRB_ERR_H_
#define MOON_MRB_ERR_H_

#include "mrb.hxx"

namespace Moon {
  bool moon_mrb_check_class(mrb_state *mrb, mrb_value obj,
                            struct RClass *klass, bool quiet);
}

#endif