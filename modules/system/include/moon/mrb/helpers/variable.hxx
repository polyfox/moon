#ifndef MOON_MRB_VARIABLE_HELPERS
#define MOON_MRB_VARIABLE_HELPERS

#include <mruby.h>
#include <mruby/variable.h>
#include <mruby/string.h>

#define IVget(_name_) mrb_iv_get(mrb, self, mrb_intern_lit(mrb, _name_))
#define IVset(_name_, _value_) mrb_iv_set(mrb, self, mrb_intern_lit(mrb, _name_), _value_)

static inline mrb_value
moon_iv_get(mrb_state* mrb, mrb_value self, const char* name)
{
  mrb_value iv_value = mrb_iv_get(mrb, self, mrb_intern_cstr(mrb, name));
  if (mrb_nil_p(iv_value)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "Cannot use a nil %S", mrb_str_new_cstr(mrb, name));
  }
  return iv_value;
}

#define KEY_VBO "@vbo"

#endif
