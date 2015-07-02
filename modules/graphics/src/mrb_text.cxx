#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/variable.h>
#include "moon/vertex_buffer.hxx"
#include "moon/api.h"
#include "moon/glm.h"
#include "moon/font.hxx"
#include "moon/shader.hxx"
#include "moon/mrb/vbo.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/font.hxx"
#include "moon/mrb/text.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/helpers.hxx"

/**
 * @method Font#generate_buffers
 */
static mrb_value
text_generate_buffers(mrb_state *mrb, mrb_value self)
{
  // string will be deleted at the end
  mrb_int outline = mrb_int(mrb, IVget("@outline"));
  mrb_float line_height = mrb_to_flo(mrb, IVget("@line_height"));
  Moon::String string(mrb_str_to_cstr(mrb, IVget("@string")));
  Moon::VertexBuffer *vbo = get_vbo(mrb, IVget(KEY_VBO));
  Moon::Font *font = get_font(mrb, IVget("@font"));
  Moon::Vector4 color = *get_vector4(mrb, IVget("@color"));
  Moon::Vector4 outline_color = *get_vector4(mrb, IVget("@outline_color"));

  vbo->Clear();
  if (outline > 0) {
    font->font->outline_type = 2;
    font->font->outline_thickness = outline;
    font->FillTextBuffer(vbo, string, outline_color, line_height);
  }
  font->font->outline_type = 0;
  font->font->outline_thickness = 0;
  font->FillTextBuffer(vbo, string, color, line_height);
  // cache size
  glm::vec2 bb = font->ComputeStringBbox(string, line_height);
  IVset("@w", mrb_fixnum_value(bb.x));
  IVset("@h", mrb_fixnum_value(bb.y));
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
  Moon::VertexBuffer *vbo = get_vbo(mrb, IVget(KEY_VBO));
  Moon::Shader *shader = get_shader(mrb, IVget(KEY_SHADER));
  Moon::Font *font = get_font(mrb, IVget("@font"));

  shader->Use();
  // model matrix - move it to the correct position in the world
  glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y + font->font->ascender, z));
  // calculate the ModelViewProjection matrix (faster to do on CPU, once for all vertices instead of per vertex)
  glm::mat4 mvp_matrix = Moon::Shader::projection_matrix * Moon::Shader::view_matrix * model_matrix;
  shader->SetUniform("mvp_matrix", mvp_matrix);

  // Set texture ID
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, font->atlas->id);
  shader->SetUniform("tex", /*GL_TEXTURE*/0);
  vbo->Render(GL_TRIANGLES);
  return self;
}

MOON_C_API void
mmrb_text_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *text_cls = mrb_define_class_under(mrb, mod, "Text", mrb->object_class);

  mrb_define_method(mrb, text_cls, "generate_buffers", text_generate_buffers, MRB_ARGS_NONE());
  mrb_define_method(mrb, text_cls, "render",           text_render,           MRB_ARGS_REQ(3));
}
