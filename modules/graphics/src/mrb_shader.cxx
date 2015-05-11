#include <mruby.h>
#include <mruby/class.h>
#include <mruby/numeric.h>
#include "moon/shader.hxx"
#include "moon/shader_loader.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/transform.hxx"

static struct RClass *shader_class;

static mrb_value
shader_s_is_legacy_get(mrb_state *mrb, mrb_value klass)
{
#ifdef MOON_GUESS_SHADER_VERSION
  return Moon::ShaderLoader::IsLegacy();
#else
  return mrb_bool_value(Moon::ShaderLoader::is_legacy);
#endif
}

static mrb_value
shader_s_is_legacy_set(mrb_state *mrb, mrb_value klass)
{
  mrb_bool leg;
  mrb_get_args(mrb, "b", &leg);
#ifndef MOON_GUESS_SHADER_VERSION
  Moon::ShaderLoader::is_legacy = leg;
#endif
  return klass;
}

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
  mrb_define_class_method(mrb, shader_class, "is_legacy",  shader_s_is_legacy_get, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, shader_class, "is_legacy=", shader_s_is_legacy_set, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, shader_class, "projection_matrix",  shader_s_projection_matrix_get, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, shader_class, "projection_matrix=", shader_s_projection_matrix_set, MRB_ARGS_REQ(1));
}
