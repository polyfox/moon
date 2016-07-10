#ifndef MMRB_TEXTURE_H
#define MMRB_TEXTURE_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/texture.hxx"
#include "moon/mrb/helpers.hxx"

#define KEY_TEXTURE "@texture"

static inline struct RClass*
mmrb_get_texture_class(mrb_state *mrb)
{
  return MOON_GET_CLASS("Texture");
}


MOON_C_API const struct mrb_data_type texture_data_type;
MOON_C_API void mmrb_texture_init(mrb_state *mrb);
MOON_C_API mrb_value mmrb_texture_load_file(mrb_state *mrb, const char *filename);

static inline Moon::Texture*
mmrb_texture_ptr(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Texture*>(mrb_data_get_ptr(mrb, self, &texture_data_type));
}

static void
mmrb_check_texture(mrb_state *mrb, Moon::Texture *texture)
{
  if (!texture->GetID()) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid texture handle.");
  }
}

static inline Moon::Texture*
mmrb_valid_texture_ptr(mrb_state *mrb, mrb_value obj)
{
  Moon::Texture *texture = mmrb_texture_ptr(mrb, obj);
  mmrb_check_texture(mrb, texture);
  return texture;
}

#endif
