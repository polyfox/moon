#include <mruby.h>
#include <mruby/class.h>
#include <mruby/numeric.h>
#include "moon/shader.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/transform.hxx"
#include "moon/mrb/helpers.hxx"

static void
shader_free(mrb_state *mrb, void *ptr)
{
  Moon::Shader *shader = static_cast<Moon::Shader*>(ptr);
  if (shader) {
    delete shader;
  }
}

MOON_C_API const struct mrb_data_type shader_data_type = { "Moon::Shader", shader_free };

static mrb_value
shader_initialize(mrb_state *mrb, mrb_value self)
{
  Moon::Shader *shader;
  char* vertexShader;
  char* fragmentShader;
  shader_free(mrb, DATA_PTR(self));
  mrb_get_args(mrb, "zz", &vertexShader, &fragmentShader);
  shader = new Moon::Shader(vertexShader, fragmentShader);
  mrb_data_init(self, shader, &shader_data_type);
  return self;
}

static mrb_value
shader_s_is_legacy_get(mrb_state *mrb, mrb_value klass)
{
  return mrb_bool_value(Moon::Shader::is_legacy);
}

static mrb_value
shader_s_is_legacy_set(mrb_state *mrb, mrb_value klass)
{
  mrb_bool leg;
  mrb_get_args(mrb, "b", &leg);
  Moon::Shader::is_legacy = leg;
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
  struct RClass *shader_class = mrb_define_class_under(mrb, mod, "Shader", mrb->object_class);
  MRB_SET_INSTANCE_TT(shader_class, MRB_TT_DATA);

  mrb_define_method(mrb, shader_class, "initialize", shader_initialize, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, shader_class, "is_legacy",  shader_s_is_legacy_get, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, shader_class, "is_legacy=", shader_s_is_legacy_set, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, shader_class, "projection_matrix",  shader_s_projection_matrix_get, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, shader_class, "projection_matrix=", shader_s_projection_matrix_set, MRB_ARGS_REQ(1));
}
