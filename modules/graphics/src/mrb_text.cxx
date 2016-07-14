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

/* Add a new text string to the VBO at the specified coordinates.
 * We use the coordinates to be able to generate alignment and layouts.
 *
 * @param [String] str the string we're adding
 * @param [Integer] x x coordinate
 * @param [Integer] y y coordinate
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

MOON_C_API void
mmrb_text_init(mrb_state *mrb)
{
  struct RClass *mod = mrb_define_module(mrb, "Moon");
  struct RClass *text_cls = mrb_define_class_under(mrb, mod, "Text", mrb->object_class);

  mrb_define_method(mrb, text_cls, "add_text", text_add_text, MRB_ARGS_REQ(3));
}
