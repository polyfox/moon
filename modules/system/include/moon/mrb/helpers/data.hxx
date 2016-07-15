#ifndef MOON_MRB_DATA_HELPERS
#define MOON_MRB_DATA_HELPERS

#include <mruby.h>
#include <mruby/data.h>

static inline void
moon_data_cleanup(mrb_state* mrb, mrb_value self)
{
  void *ptr = DATA_PTR(self);
  const struct mrb_data_type* type = DATA_TYPE(self);
  if (ptr && type) {
    type->dfree(mrb, ptr);
  }
  DATA_PTR(self) = NULL;
}

#endif
