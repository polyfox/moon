#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include "moon/api.h"
#include "moon/mrb/helpers.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/vbo.hxx"
#include "moon/shader.hxx"
#include "moon/texture.hxx"
#include "moon/vector2.hxx"
#include "moon/vector3.hxx"
#include "moon/vertex_buffer.hxx"

static mrb_value
tilemap_render(mrb_state *mrb, mrb_value self)
{
  mrb_float x, y, z;
  mrb_get_args(mrb, "fff", &x, &y, &z);
  const GLfloat opacity = mrb_to_flo(mrb, moon_mrb_iv_get_no_nil(mrb, self, "@opacity"));
  const GLfloat angle = mrb_to_flo(mrb, moon_mrb_iv_get_no_nil(mrb, self, "@angle"));
  Moon::VertexBuffer *vbo = get_vbo(mrb, moon_mrb_iv_get_no_nil(mrb, self, KEY_VBO));
  Moon::Shader *shader = get_shader(mrb, moon_mrb_iv_get_no_nil(mrb, self, KEY_SHADER));
  Moon::Texture *texture = get_valid_texture(mrb, moon_mrb_iv_get_no_nil(mrb, self, KEY_TEXTURE));
  Moon::Vector2 origin(*get_vector2(mrb, moon_mrb_iv_get_no_nil(mrb, self, KEY_ORIGIN)));
  Moon::Vector3 position(x, y, z);

  glm::mat4 rotation_matrix = moon_rotate(angle, origin);
  //model matrix - move it to the correct position in the world
  glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), position);
  // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
  glm::mat4 mvp_matrix = Moon::Shader::projection_matrix * Moon::Shader::view_matrix * model_matrix * rotation_matrix;

  glActiveTexture(GL_TEXTURE0);
  texture->Bind();
  shader->Use();
  shader->SetUniform("mvp_matrix", mvp_matrix);
  shader->SetUniform("opacity", opacity);
  shader->SetUniform("color", Moon::Vector4(1, 1, 1, 1));
  shader->SetUniform("tone", Moon::Vector4(0, 0, 0, 1));
  shader->SetUniform("tex", /*GL_TEXTURE*/0);
  // TRIANGLE_STRIPS aren't very friendly with multiple quads
  vbo->Render(GL_TRIANGLES, 0);
  return self;
}

MOON_C_API void
mmrb_tilemap_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *tilemap_cls = mrb_define_class_under(mrb, mod, "Tilemap", mrb->object_class);

  mrb_define_method(mrb, tilemap_cls, "render", tilemap_render, MRB_ARGS_REQ(3));
}
