#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include "moon/glm.h"
#include "moon/api.h"
#include "moon/shader.hxx"
#include "moon/texture.hxx"
#include "moon/vector2.hxx"
#include "moon/vector3.hxx"
#include "moon/vertex_buffer.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/vertex_buffer.hxx"
#include "moon/mrb/helpers.hxx"

static mrb_value
tilemap_render(mrb_state *mrb, mrb_value self)
{
  mrb_float x, y, z;
  mrb_get_args(mrb, "fff", &x, &y, &z);
  const GLfloat opacity = mrb_to_flo(mrb, moon_iv_get(mrb, self, "@opacity"));
  const GLfloat angle = mrb_to_flo(mrb, moon_iv_get(mrb, self, "@angle"));
  Moon::VertexBuffer *vbo = mmrb_vertex_buffer_ptr(mrb, moon_iv_get(mrb, self, KEY_VBO));
  Moon::Shader *shader = mmrb_shader_ptr(mrb, moon_iv_get(mrb, self, KEY_SHADER));
  Moon::Texture *texture = mmrb_valid_texture_ptr(mrb, moon_iv_get(mrb, self, KEY_TEXTURE));
  Moon::Vector2 origin(*mmrb_vector2_ptr(mrb, moon_iv_get(mrb, self, KEY_ORIGIN)));
  Moon::Vector3 position(x, y, z);

  glm::mat4 rotation_matrix = moon_rotate(angle, origin);
  //model matrix - move it to the correct position in the world
  glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), position);
  // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
  glm::mat4 mvp_matrix = Moon::Shader::projection_matrix * Moon::Shader::view_matrix * model_matrix * rotation_matrix;

  glActiveTexture(GL_TEXTURE0);
  texture->Bind();
  shader->Use();
  shader->SetUniform(std::string("mvp_matrix"), mvp_matrix);
  shader->SetUniform(std::string("opacity"), opacity);
  shader->SetUniform(std::string("color"), Moon::Vector4(1, 1, 1, 1));
  shader->SetUniform(std::string("tone"), Moon::Vector4(0, 0, 0, 1));
  shader->SetUniform(std::string("tex"), /*GL_TEXTURE*/0);
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
