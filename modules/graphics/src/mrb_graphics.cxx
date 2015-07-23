#include <mruby.h>
#include <mruby/class.h>
#include "moon/mrb/vertex_buffer.hxx"
#include "moon/mrb/font.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/sprite.hxx"
#include "moon/mrb/spritesheet.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/text.hxx"

MOON_C_API void mmrb_tilemap_init(mrb_state *mrb, struct RClass* mod);
MOON_C_API void
mrb_mruby_moon_graphics_gem_init(mrb_state* mrb)
{
  struct RClass *moon_module = mrb_define_module(mrb, "Moon");
  mmrb_vbo_init(mrb, moon_module);
  mmrb_shader_init(mrb, moon_module);
  mmrb_font_init(mrb, moon_module);
  mmrb_text_init(mrb, moon_module);
  mmrb_sprite_init(mrb, moon_module);
  mmrb_spritesheet_init(mrb, moon_module);
  mmrb_texture_init(mrb, moon_module);
  mmrb_tilemap_init(mrb, moon_module);
}

MOON_C_API void
mrb_mruby_moon_graphics_gem_final(mrb_state* mrb)
{
  //
}
