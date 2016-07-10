#include <mruby.h>
#include <mruby/class.h>
#include "moon/mrb/vertex_buffer.hxx"
#include "moon/mrb/font.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/texture.hxx"

MOON_C_API void mmrb_sprite_init(mrb_state *mrb);
MOON_C_API void mmrb_spritesheet_init(mrb_state *mrb);
MOON_C_API void mmrb_text_init(mrb_state *mrb);

MOON_C_API void
mrb_mruby_moon_graphics_gem_init(mrb_state* mrb)
{
  struct RClass *moon_module = mrb_define_module(mrb, "Moon");
  mmrb_vbo_init(mrb);
  mmrb_shader_init(mrb);
  mmrb_font_init(mrb);
  mmrb_text_init(mrb);
  mmrb_sprite_init(mrb);
  mmrb_spritesheet_init(mrb);
  mmrb_texture_init(mrb);
}

MOON_C_API void
mrb_mruby_moon_graphics_gem_final(mrb_state* mrb)
{
  //
}
