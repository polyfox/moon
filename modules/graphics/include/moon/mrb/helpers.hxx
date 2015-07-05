#ifndef MOON_MRB_HELPERS
#define MOON_MRB_HELPERS

#include <mruby.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include "moon/rect.hxx"
#include "moon/shader.hxx"
#include "moon/texture.hxx"
#include "moon/transform.hxx"
#include "moon/font.hxx"
#include "moon/vector2.hxx"
#include "moon/vector3.hxx"
#include "moon/vector4.hxx"
#include "moon/vertex_buffer.hxx"
#include "moon/mrb/rect.hxx"
#include "moon/mrb/font.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/transform.hxx"
#include "moon/mrb/vbo.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"

#define IVget(_name_) mrb_iv_get(mrb, self, mrb_intern_lit(mrb, _name_))
#define IVset(_name_, _value_) mrb_iv_set(mrb, self, mrb_intern_lit(mrb, _name_), _value_)

#define KEY_TEXTURE "@texture"
#define KEY_SHADER "@shader"
#define KEY_VBO "@vbo"

#define TEXTURE_CLASS mrb_module_get_under(mrb, mrb_module_get(mrb, "Moon"), "Texture")
;

static inline Moon::IntRect*
get_rect(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::IntRect*>(mrb_data_get_ptr(mrb, self, &rect_data_type));
}

static inline Moon::Vector2*
get_vector2(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Vector2*>(mrb_data_get_ptr(mrb, self, &vector2_data_type));
}

static inline Moon::Vector3*
get_vector3(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Vector3*>(mrb_data_get_ptr(mrb, self, &vector3_data_type));
}

static inline Moon::Vector4*
get_vector4(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Vector4*>(mrb_data_get_ptr(mrb, self, &vector4_data_type));
}

static inline Moon::Transform*
get_transform(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Transform*>(mrb_data_get_ptr(mrb, self, &transform_data_type));
}

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

static inline Moon::Font*
get_font(mrb_state *mrb, mrb_value self)
{
  return static_cast<Moon::Font*>(mrb_data_get_ptr(mrb, self, &font_data_type));
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

static inline glm::mat4 moon_rotate(float angle, const glm::vec2 origin) {
  return glm::translate(glm::rotate(
    glm::mat4(1.0f),
    glm::radians(angle),
    glm::vec3(0, 0, 1)
  ), glm::vec3(-origin.x, -origin.y, 0));
}

#endif
