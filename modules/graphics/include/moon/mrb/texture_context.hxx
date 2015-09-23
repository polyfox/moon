#ifndef MMRB_TEXTURE_CONTEXT_H
#define MMRB_TEXTURE_CONTEXT_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/texture.hxx"
#include "moon/texture_context.hxx"
#include "moon/mrb/helpers.hxx"

MOON_C_API const struct mrb_data_type texture_ctx_data_type;

static inline Moon::TextureContext*
mmrb_texture_ctx_ptr(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::TextureContext*>(mrb_data_get_ptr(mrb, self, &texture_ctx_data_type));
}

#endif
