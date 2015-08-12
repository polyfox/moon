#include <mruby.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/variable.h>
#include "moon/mrb/font.hxx"
#include "moon/mrb/matrix4.hxx"
#include "moon/mrb/rect.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/vertex_buffer.hxx"

static mrb_value
apply_transform(mrb_state *mrb, mrb_value self)
{
  Moon::Shader *shader;
  Moon::Matrix4 *matrix4;
  mrb_get_args(mrb, "dd",
    &shader, &shader_data_type,
    &matrix4, &matrix4_data_type);

  glm::mat4 mvp_matrix = Moon::Shader::projection_matrix * Moon::Shader::view_matrix * (*matrix4);

  shader->SetUniform("mvp_matrix", mvp_matrix);
  return self;
}

static mrb_value
bind_texture(mrb_state *mrb, mrb_value self)
{
  void *ptr;
  Moon::Shader *shader;
  mrb_value texture_obj;
  mrb_get_args(mrb, "do", &shader, &shader_data_type, &texture_obj);
  glActiveTexture(GL_TEXTURE0);
  if ((ptr = mrb_data_check_get_ptr(mrb, texture_obj, &texture_data_type))) {
    Moon::Texture *texture = static_cast<Moon::Texture*>(ptr);
    mmrb_check_texture(mrb, texture);
    texture->Bind();
  } else if ((ptr = mrb_data_check_get_ptr(mrb, texture_obj, &font_data_type))) {
    Moon::Font *font = static_cast<Moon::Font*>(ptr);
    font->Bind();
  } else {
    mrb_raisef(mrb, E_TYPE_ERROR, "Cannot bind texture for %S", mrb_obj_classname(mrb, texture_obj));
    return mrb_nil_value();
  }
  shader->SetUniform("tex", /*GL_TEXTURE*/0);
  return self;
}

MOON_C_API void
mmrb_renderer_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *renderer_cls = mrb_define_class_under(mrb, mod, "Renderer", mrb->object_class);

  mrb_define_method(mrb, renderer_cls, "bind_texture",    bind_texture,      MRB_ARGS_REQ(2));
  mrb_define_method(mrb, renderer_cls, "apply_transform", apply_transform,   MRB_ARGS_REQ(2));
}
