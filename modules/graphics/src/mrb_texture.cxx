#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include <mruby/string.h>
#include "moon/mrb/texture.hxx"
#include "moon/texture.hxx"
#include "moon/api.h"

static struct RClass *texture_class;

static void
texture_free(mrb_state *mrb, void *p)
{
  Moon::Texture *texture = (Moon::Texture*)p;
  if (texture) {
    delete(texture);
  }
}

MOON_C_API const struct mrb_data_type texture_data_type = { "Texture", texture_free };

static inline Moon::Texture*
get_texture(mrb_state *mrb, mrb_value self)
{
  return (Moon::Texture*)mrb_data_get_ptr(mrb, self, &texture_data_type);
}

MOON_C_API mrb_value
mmrb_texture_load_file(mrb_state *mrb, const char *filename)
{
  mrb_value fn = mrb_str_new_cstr(mrb, filename);
  return mrb_obj_new(mrb, texture_class, 1, &fn);
}

static mrb_value
texture_initialize(mrb_state *mrb, mrb_value self)
{
  char* filename;
  Moon::Texture *texture;
  mrb_get_args(mrb, "z", &filename);

  texture = (Moon::Texture*)DATA_PTR(self);
  if (texture) {
    texture_free(mrb, (void*)texture);
    texture = NULL;
  }
  DATA_PTR(self) = NULL;

  if (exists(filename)) {
    texture = new Moon::Texture(filename);
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
  return mrb_float_value(mrb, get_texture(mrb, self)->GetWidth());
}

static mrb_value
texture_height(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_texture(mrb, self)->GetHeight());
}

MOON_C_API void
mmrb_texture_init(mrb_state *mrb, struct RClass* mod)
{
  texture_class = mrb_define_class_under(mrb, mod, "Texture", mrb->object_class);
  MRB_SET_INSTANCE_TT(texture_class, MRB_TT_DATA);

  mrb_define_method(mrb, texture_class, "initialize", texture_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, texture_class, "w",      texture_width,      MRB_ARGS_NONE());
  mrb_define_method(mrb, texture_class, "h",     texture_height,     MRB_ARGS_NONE());
}
