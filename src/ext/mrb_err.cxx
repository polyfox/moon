#include "mrb.hxx"
#include "mrb_err.hxx"

namespace Moon
{
  bool
  moon_mrb_check_class(mrb_state *mrb, mrb_value obj, struct RClass *klass, bool quiet)
  {
    if (mrb_obj_class(mrb, obj) != klass) {
      if (quiet)
        return false;
      else {
        mrb_raisef(mrb, E_TYPE_ERROR,
                   "expected %s but recieved %s",
                   mrb_class_name(mrb, klass), mrb_obj_classname(mrb, obj));
      }
    }
    return true;
  }
}