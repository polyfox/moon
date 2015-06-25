#ifndef MOON_MRB_HELPERS
#define MOON_MRB_HELPERS

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include "moon/shader.hxx"
#include "moon/texture.hxx"
#include "moon/vertex_buffer.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/vbo.hxx"

#define IVget(_name_) mrb_iv_get(mrb, self, mrb_intern_lit(mrb, _name_))
#define IVset(_name_, _value_) mrb_iv_set(mrb, self, mrb_intern_lit(mrb, _name_), _value_)

#define KEY_TEXTURE "@texture"
#define KEY_SHADER "@shader"
#define KEY_VBO "@vbo"

#define TEXTURE_CLASS mrb_module_get_under(mrb, mrb_module_get(mrb, "Moon"), "Texture")
;

static inline Moon::Texture*
get_texture(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Texture*>(mrb_data_get_ptr(mrb, self, &texture_data_type));
}

static inline Moon::Shader*
get_shader(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Shader*>(mrb_data_get_ptr(mrb, self, &shader_data_type));
}

static inline Moon::VertexBuffer*
get_vbo(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::VertexBuffer*>(mrb_data_get_ptr(mrb, self, &vbo_data_type));
}

static inline Moon::Texture*
get_valid_texture(mrb_state *mrb, mrb_value obj)
{
  Moon::Texture *texture = get_texture(mrb, obj);
  if (!texture->GetID()) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid texture handle.");
  }
  return texture;
}

#endif
