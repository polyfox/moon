#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/numeric.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include "moon/glm.h"
#include "moon/api.h"
#include "moon/mrb/rect.hxx"
#include "moon/mrb/shader.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/mrb/vertex_buffer.hxx"
#include "moon/mrb/helpers.hxx"
#include "moon/mrb_err.hxx"

static mrb_value
sprite_generate_buffers(mrb_state *mrb, mrb_value self)
{
  Moon::Texture *texture = mmrb_valid_texture_ptr(mrb, moon_iv_get(mrb, self, KEY_TEXTURE));
  Moon::VertexBuffer *vbo = mmrb_vertex_buffer_ptr(mrb, moon_iv_get(mrb, self, KEY_VBO));
  mrb_value clip = IVget("@clip_rect");

  vbo->Clear();
  //Texture coordinates
  GLfloat s0 = 0.f;
  GLfloat s1 = 1.f;
  GLfloat t0 = 0.f;
  GLfloat t1 = 1.f;

  //Vertex coordinates
  GLfloat width = texture->GetWidth();
  GLfloat height = texture->GetHeight();

  //Handle clipping
  if (!mrb_nil_p(clip)) {
    // TODO: get C++ clip rect from mrb rect
    Moon::IntRect *clip_rect = mmrb_rect_ptr(mrb, clip);

    //Texture coordinates
    s0 = (float)clip_rect->x / texture->GetWidth();
    s1 = (float)(clip_rect->x + clip_rect->w) / texture->GetWidth();
    t0 = (float)clip_rect->y / texture->GetHeight();
    t1 = (float)(clip_rect->y + clip_rect->h) / texture->GetHeight();
    //Vertex coordinates
    width = clip_rect->w;
    height = clip_rect->h;
  }

  Moon::Vertex vertices[4] = {
    { {0.f,   0.f},    {s0, t0}, Moon::Vector4(1, 1, 1, 1) },
    { {width, 0.f},    {s1, t0}, Moon::Vector4(1, 1, 1, 1) },
    { {width, height}, {s1, t1}, Moon::Vector4(1, 1, 1, 1) },
    { {0.f,   height}, {s0, t1}, Moon::Vector4(1, 1, 1, 1) }
  };
  GLuint indices[4] = {0, 1, 3, 2}; // rendering indices
  vbo->PushBack(vertices, 4, indices, 4);

  return self;
}

MOON_C_API void
mmrb_sprite_init(mrb_state *mrb)
{
  struct RClass *mod = mrb_define_module(mrb, "Moon");
  struct RClass *sprite_class = mrb_define_class_under(mrb, mod, "Sprite", mrb->object_class);
  mrb_define_method(mrb, sprite_class, "generate_buffers", sprite_generate_buffers, MRB_ARGS_NONE());
}
