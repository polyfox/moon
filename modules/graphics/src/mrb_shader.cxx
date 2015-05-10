#include <mruby.h>
#include <mruby/class.h>
#include <mruby/numeric.h>
#include "moon/shader.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/transform.hxx"

static struct RClass *shader_class;

static mrb_value
shader_s_projection_matrix_get(mrb_state *mrb, mrb_value klass)
{
  return mmrb_transform_value(mrb, Moon::Shader::projection_matrix);
}

static mrb_value
shader_s_projection_matrix_set(mrb_state *mrb, mrb_value klass)
{
  Moon::Transform *mat;
  mrb_get_args(mrb, "d", &mat, &transform_data_type);
  Moon::Shader::projection_matrix = *mat;
  return klass;
}

MOON_C_API void
mmrb_shader_init(mrb_state *mrb, struct RClass* mod)
{
  shader_class = mrb_define_class_under(mrb, mod, "Shader", mrb->object_class);
  mrb_define_class_method(mrb, shader_class, "projection_matrix",  shader_s_projection_matrix_get, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, shader_class, "projection_matrix=", shader_s_projection_matrix_set, MRB_ARGS_REQ(1));
}
