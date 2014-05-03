#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/mrb/texture.hxx"
#include "moon/mrb/texture_context.hxx"
#include "moon/mrb/vector4.hxx"
#include "moon/rect.hxx"
#include "moon/texture.hxx"
#include "moon/vector4.hxx"

static void
texture_ctx_free(mrb_state *mrb, void *p)
{
  Moon::TextureContext *ctx = (Moon::TextureContext*)p;
  if (ctx) {
    delete(ctx);
  }
}

MOON_C_API const struct mrb_data_type texture_ctx_data_type = { "TextureContext", texture_free };

static mrb_value
texture_ctx_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value tex;
  Moon::Texture *texture;
  Moon::TextureContext *context;
  mrb_get_args(mrb, "o", &tex);

  texture = mmrb_valid_texture_ptr(mrb, tex);
  context = new Moon::TextureContext(texture);
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, KEY_TEXTURE), tex);

  mrb_data_init(self, context, &context_data_type);
  return mrb_nil_value();
}

static mrb_value
texture_ctx_get_pixel(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y;
  mrb_get_args(mrb, "ii", &x, &y);
  Moon::TextureContext *context = mmrb_texture_ctx_ptr(mrb, self);
  Moon::Vector4 col = context->get_pixel(x, y);
  return mmrb_vector4_value(mrb, col);
}

static mrb_value
texture_ctx_set_pixel(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y;
  mrb_value rcolor;
  mrb_get_args(mrb, "iio", &x, &y, &rcolor);
  Moon::TextureContext *context = mmrb_texture_ctx_ptr(mrb, self);
  context->set_pixel(x, y, mmrb_to_vector4(mrb, rcolor));
  return mrb_nil_value();
}

static mrb_value
texture_ctx_fill_rect(mrb_state *mrb, mrb_value self)
{
  mrb_value rrect;
  mrb_value rcolor;
  mrb_get_args(mrb, "oo", &rrect, &rcolor);
  Moon::TextureContext *context = mmrb_texture_ctx_ptr(mrb, self);
  context->fill_rect(mmrb_to_rect(mrb, rrect), mmrb_to_vector4(mrb, rcolor));
  return mrb_nil_value();
}

static mrb_value
texture_ctx_destroy(mrb_state *mrb, mrb_value self)
{
  texture_ctx_free(mrb, mmrb_texture_ctx_ptr(mrb, self));
  DATA_PTR(self) = NULL;
  DATA_TYPE(self) = NULL;
  return mrb_nil_value();
}

MOON_C_API void
mmrb_texture_ctx_init(mrb_state *mrb, struct RClass* mod)
{
  struct RClass *texture_ctx_class = mrb_define_class_under(mrb, mod, "TextureContext", mrb->object_class);
  MRB_SET_INSTANCE_TT(texture_ctx_class, MRB_TT_DATA);

  mrb_define_method(mrb, texture_ctx_class, "initialize", texture_ctx_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, texture_ctx_class, "[]",         texture_ctx_get_pixel,  MRB_ARGS_REQ(2));
  mrb_define_method(mrb, texture_ctx_class, "[]=",        texture_ctx_set_pixel,  MRB_ARGS_REQ(3));
  mrb_define_method(mrb, texture_ctx_class, "fill_rect",  texture_ctx_fill_rect,  MRB_ARGS_REQ(2));
  mrb_define_method(mrb, texture_ctx_class, "destroy",    texture_ctx_destroy,    MRB_ARGS_NONE());
}
