#ifndef MMRB_SHADER_H
#define MMRB_SHADER_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/shader.hxx"

MOON_C_API const struct mrb_data_type shader_data_type;
MOON_C_API void mmrb_shader_init(mrb_state *mrb);

static inline Moon::Shader*
mmrb_shader_ptr(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Shader*>(mrb_data_get_ptr(mrb, self, &shader_data_type));
}

#endif
