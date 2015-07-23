#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/variable.h>
#include "moon/glm.h"
#include "moon/api.h"
#include "moon/font.hxx"
#include "moon/shader.hxx"
#include "moon/vertex_buffer.hxx"
#include "moon/mrb/font.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/vertex_buffer.hxx"
#include "moon/mrb/helpers.hxx"

/**
 * @method Font#add_text(text, x, y)
 */
static mrb_value
text_add_text(mrb_state *mrb, mrb_value self)
{
  char *str;
  mrb_float x, y;
  mrb_get_args(mrb, "zff", &str, &x, &y);
  // string will be deleted at the end
  mrb_int outline = mrb_int(mrb, moon_iv_get(mrb, self, "@outline"));
  mrb_float line_height = mrb_to_flo(mrb, moon_iv_get(mrb, self, "@line_height"));
  Moon::VertexBuffer *vbo = mmrb_vertex_buffer_ptr(mrb, moon_iv_get(mrb, self, KEY_VBO));
  Moon::Font *font = mmrb_font_ptr(mrb, moon_iv_get(mrb, self, "@font"));
  Moon::Vector4 color = *mmrb_vector4_ptr(mrb, moon_iv_get(mrb, self, "@color"));
  Moon::Vector4 outline_color = *mmrb_vector4_ptr(mrb, moon_iv_get(mrb, self, "@outline_color"));
  Moon::String string(str);

  if (outline > 0) {
    font->font->outline_type = 2;
    font->font->outline_thickness = outline;
    font->FillTextBuffer(vbo, string, outline_color, x, y, line_height);
  }
  font->font->outline_type = 0;
  font->font->outline_thickness = 0;
  font->FillTextBuffer(vbo, string, color, x, y, line_height);
  return self;
}

/**
 * @method Font#render(x, y, z)
 * @param [Float] x
 * @param [Float] y
 * @param [Float] z
 */
static mrb_value
text_render(mrb_state *mrb, mrb_value self)
{
  mrb_float x, y, z;
  mrb_get_args(mrb, "fff", &x, &y, &z);
  Moon::VertexBuffer *vbo = mmrb_vertex_buffer_ptr(mrb, moon_iv_get(mrb, self, KEY_VBO));
  Moon::Shader *shader = mmrb_shader_ptr(mrb, moon_iv_get(mrb, self, KEY_SHADER));
  Moon::Font *font = mmrb_font_ptr(mrb, moon_iv_get(mrb, self, "@font"));

  shader->Use();
  // model matrix - move it to the correct position in the world
  glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y + font->font->ascender, z));
  // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
  glm::mat4 mvp_matrix = Moon::Shader::projection_matrix * Moon::Shader::view_matrix * model_matrix;
  shader->SetUniform("mvp_matrix", mvp_matrix);

  // Set texture ID
  glActiveTexture(GL_TEXTURE0);
  font->Bind();
  shader->SetUniform("tex", /*GL_TEXTURE*/0);
  vbo->Render(GL_TRIANGLES);
  return self;
}

MOON_C_API void
mmrb_text_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *text_cls = mrb_define_class_under(mrb, mod, "Text", mrb->object_class);

  mrb_define_method(mrb, text_cls, "add_text", text_add_text, MRB_ARGS_REQ(3));
  mrb_define_method(mrb, text_cls, "render",   text_render,   MRB_ARGS_REQ(3));
}
