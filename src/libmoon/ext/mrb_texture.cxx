#include "moon/mrb/texture.hxx"
#include "moon/texture.hxx"
#include "moon/mrb/shared_types.hxx"

using Moon::Texture;

static void
texture_free(mrb_state *mrb, void *p)
{
  moon_texture *texture = (moon_texture*)p;
  if (texture) {
    delete(texture);
  }
}

struct mrb_data_type texture_data_type = { "Texture", texture_free };

static mrb_value
texture_initialize(mrb_state *mrb, mrb_value self)
{
  char* filename;
  moon_texture *texture;
  mrb_get_args(mrb, "z", &filename);

  texture = (moon_texture*)DATA_PTR(self);
  if (texture) {
    texture_free(mrb, (void*)texture);
    texture = NULL;
  }
  DATA_PTR(self) = NULL;

  if (exists(filename)) {
    texture = new moon_texture(Texture::load(filename));
  } else {
    mrb_raisef(mrb, E_SCRIPT_ERROR,
               "cannot load such file -- %S",
               mrb_str_new_cstr(mrb, filename));
  }

  mrb_data_init(self, texture, &texture_data_type);

  return self;
}

static mrb_value
texture_width(mrb_state *mrb, mrb_value self)
{
  moon_texture *texture;
  texture = (moon_texture*)mrb_data_get_ptr(mrb, self, &texture_data_type);

  return mrb_float_value(mrb, (*texture)->width());
}

static mrb_value
texture_height(mrb_state *mrb, mrb_value self)
{
  moon_texture *texture;
  texture = (moon_texture*)mrb_data_get_ptr(mrb, self, &texture_data_type);

  return mrb_float_value(mrb, (*texture)->height());
}

struct RClass*
mmrb_texture_init(mrb_state *mrb)
{
  struct RClass *texture_class;
  texture_class = mrb_define_class_under(mrb, mmrb_Moon, "Texture", mrb->object_class);
  MRB_SET_INSTANCE_TT(texture_class, MRB_TT_DATA);

  mrb_define_method(mrb, texture_class, "initialize", texture_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, texture_class, "width",      texture_width,      MRB_ARGS_NONE());
  mrb_define_method(mrb, texture_class, "height",     texture_height,     MRB_ARGS_NONE());

  return texture_class;
}
