#include <stdbool.h>
#include <mruby.h>
#include <mruby/class.h>
#include "moon/mrb_err.hxx"

bool
mmrb_check_class(mrb_state *mrb, mrb_value obj, struct RClass *klass, bool quiet)
{
  if (mrb_obj_class(mrb, obj) != klass) {
    if (quiet)
      return false;
    else {
      mrb_raisef(mrb, E_TYPE_ERROR,
                 "recieved %s (expected %s)",
                 mrb_obj_classname(mrb, obj),
                 mrb_class_name(mrb, klass));
    }
  }
  return true;
}
