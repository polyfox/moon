#ifndef MOON_MRB_HELPERS
#define MOON_MRB_HELPERS

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#define IVget(_name_) mrb_iv_get(mrb, self, mrb_intern_lit(mrb, _name_))
#define IVset(_name_, _value_) mrb_iv_set(mrb, self, mrb_intern_lit(mrb, _name_), _value_)

#define KEY_SHADER "@shader"
#define KEY_VBO "@vbo"
#define KEY_ORIGIN "@origin"

#define MOON_GET_CLASS(__name__) mrb_class_get_under(mrb, mrb_module_get(mrb, "Moon"), __name__)
#define MOON_GET_MODULE(__name__) mrb_module_get_under(mrb, mrb_module_get(mrb, "Moon"), __name__)
;

static inline mrb_value
moon_iv_get(mrb_state *mrb, mrb_value self, const char *name)
{
  mrb_value iv_value = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, name));
  if (mrb_nil_p(iv_value)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "Cannot use a nil %S", mrb_str_new_cstr(mrb, name));
  }
  return iv_value;
}

#endif
