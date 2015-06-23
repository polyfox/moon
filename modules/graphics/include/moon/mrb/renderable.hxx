#ifndef MOON_MRB_RENDERABLE
#define MOON_MRB_RENDERABLE

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include "moon/shader.hxx"
#include "moon/mrb/helpers.hxx"

template <class T>
static void
renderable_shader_set_from_rval(mrb_state *mrb, mrb_value self, mrb_value new_shader)
{
  T *renderable = static_cast<T*>(DATA_PTR(self));
  assert(renderable);
  renderable->shader = get_shader(mrb, new_shader);
  IVset(KEY_SHADER, new_shader);
}

template <class T>
static void
renderable_initialize_shader(mrb_state *mrb, mrb_value self)
{
  renderable_shader_set_from_rval<T>(mrb, self, mrb_funcall(mrb, self, "shader", 0));
}

template <class T>
static mrb_value
renderable_shader_set(mrb_state *mrb, mrb_value self)
{
  mrb_value new_shader;
  mrb_get_args(mrb, "o", &new_shader);
  renderable_shader_set_from_rval<T>(mrb, self, new_shader);
  return self;
}

#endif
