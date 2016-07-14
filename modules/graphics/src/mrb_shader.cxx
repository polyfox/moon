#include <mruby.h>
#include <mruby/class.h>
#include <mruby/numeric.h>
#include "moon/shader.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/vector1.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/matrix4.hxx"
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

/* Initializes a new shader object, generating a vertex and fragment shader pair.
 *
 * @param [String] vertexShader contents of the vertex shader program
 * @param [String] fragmentShader contents of the fragment shader program
 */
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

/*
 * Set as the currently active shader. We use this in render methods to activate
 * the shader before rendering.
 *
 * @return [self]
 */
static mrb_value
shader_use(mrb_state *mrb, mrb_value self)
{
  mmrb_shader_ptr(mrb, self)->Use();
  return self;
}

/*
 * Pass uniform data into the shader.
 *
 * @param [String] cname Name of the uniform.
 * @param [Float, Fixnum, Vector2, Vector3, Vector4, Matrix4] data
 * @return [self]
 */
static mrb_value
shader_set_uniform(mrb_state *mrb, mrb_value self)
{
  char *cname;
  mrb_value data;
  Moon::Shader *shader;
  mrb_get_args(mrb, "zo", &cname, &data);
  shader = mmrb_shader_ptr(mrb, self);
  std::string name(cname);

  switch (mrb_type(data)) {
    case MRB_TT_FIXNUM: {
      shader->SetUniform(name, static_cast<GLint>(mrb_int(mrb, data)));
    } break;
    case MRB_TT_FLOAT: {
      shader->SetUniform(name, static_cast<GLfloat>(mrb_to_flo(mrb, data)));
    } break;
    case MRB_TT_DATA: {
      void *ptr;
      /*if ((ptr = mrb_data_check_get_ptr(mrb, data, &vector1_data_type))) {
        shader->SetUniform(name, *static_cast<Moon::Vector1*>(ptr));

      } else */if ((ptr = mrb_data_check_get_ptr(mrb, data, &vector2_data_type))) {
        shader->SetUniform(name, *static_cast<Moon::Vector2*>(ptr));

      } else if ((ptr = mrb_data_check_get_ptr(mrb, data, &vector3_data_type))) {
        shader->SetUniform(name, *static_cast<Moon::Vector3*>(ptr));

      } else if ((ptr = mrb_data_check_get_ptr(mrb, data, &vector4_data_type))) {
        shader->SetUniform(name, *static_cast<Moon::Vector4*>(ptr));

      } else if ((ptr = mrb_data_check_get_ptr(mrb, data, &matrix4_data_type))) {
        shader->SetUniform(name, *static_cast<Moon::Matrix4*>(ptr));

      } else {
        mrb_raisef(mrb, E_TYPE_ERROR, "unexpected object %S (expected Vector4 or Matrix4)", mrb_obj_classname(mrb, data));
      }
    } break;
    default:
      mrb_raisef(mrb, E_TYPE_ERROR, "unexpected object %S", mrb_obj_classname(mrb, data));
      break;
  }
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

MOON_C_API void
mmrb_shader_init(mrb_state *mrb)
{
  struct RClass *mod = mrb_define_module(mrb, "Moon");
  struct RClass *shader_class = mrb_define_class_under(mrb, mod, "Shader", mrb->object_class);
  MRB_SET_INSTANCE_TT(shader_class, MRB_TT_DATA);

  mrb_define_method(mrb, shader_class, "initialize",  shader_initialize,  MRB_ARGS_REQ(2));
  mrb_define_method(mrb, shader_class, "use",         shader_use,         MRB_ARGS_NONE());
  mrb_define_method(mrb, shader_class, "set_uniform", shader_set_uniform, MRB_ARGS_REQ(2));

  mrb_define_class_method(mrb, shader_class, "is_legacy",  shader_s_is_legacy_get, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, shader_class, "is_legacy=", shader_s_is_legacy_set, MRB_ARGS_REQ(1));
}
