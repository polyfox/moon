#include <mruby.h>
#include <mruby/class.h>
#include "moon/mrb/font.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/sprite.hxx"
#include "moon/mrb/spritesheet.hxx"
#include "moon/mrb/texture.hxx"

static struct RClass *moon_module;

MOON_C_API void
mrb_mruby_moon_graphics_gem_init(mrb_state* mrb)
{
  moon_module = mrb_define_module(mrb, "Moon");
  mmrb_shader_init(mrb, moon_module);
  mmrb_font_init(mrb, moon_module);
  mmrb_sprite_init(mrb, moon_module);
  mmrb_spritesheet_init(mrb, moon_module);
  mmrb_texture_init(mrb, moon_module);
}

MOON_C_API void
mrb_mruby_moon_graphics_gem_final(mrb_state* mrb)
{
  //
}