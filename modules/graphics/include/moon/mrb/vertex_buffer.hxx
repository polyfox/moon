#ifndef MMRB_VBO_H
#define MMRB_VBO_H

#include "moon/api.h"
#include "moon/vertex_buffer.hxx"
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>

MOON_C_API const struct mrb_data_type vbo_data_type;
MOON_C_API void mmrb_vbo_init(mrb_state* mrb);

static inline Moon::VertexBuffer*
mmrb_vertex_buffer_ptr(mrb_state* mrb, mrb_value self)
{
  return static_cast<Moon::VertexBuffer*>(
    mrb_data_get_ptr(mrb, self, &vbo_data_type));
}

#endif
