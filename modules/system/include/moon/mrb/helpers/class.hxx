#ifndef MOON_MRB_CLASS_HELPERS
#define MOON_MRB_CLASS_HELPERS

#include <mruby.h>
#include <mruby/class.h>

#define MOON_GET_CLASS(__name__) mrb_class_get_under(mrb, mrb_module_get(mrb, "Moon"), __name__)
#define MOON_GET_MODULE(__name__) mrb_module_get_under(mrb, mrb_module_get(mrb, "Moon"), __name__)

#endif
