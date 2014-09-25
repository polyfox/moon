#include "moon/mrb/texture.hxx"
#include "moon/texture.hxx"
#include "moon/mrb/shared_types.hxx"

using Moon::Texture;

static void texture_deallocate(mrb_state *mrb, void *p) {
  delete((moon_texture*)p);
};

struct mrb_data_type texture_data_type = {
  "Texture", texture_deallocate,
};

static mrb_value texture_initialize(mrb_state *mrb, mrb_value self) {
  char* filename;
  mrb_get_args(mrb, "z", &filename);

  // ugly hack to make a pointer to shared_ptr
  auto texture = new moon_texture(Texture::load(filename));

  DATA_TYPE(self) = &texture_data_type;
  DATA_PTR(self) = texture;

  return mrb_nil_value();
};

static mrb_value texture_width(mrb_state *mrb, mrb_value self) {
  moon_texture* texture;
  Data_Get_Struct(mrb, self, &texture_data_type, texture);

  return mrb_float_value(mrb, (*texture)->width());
}

static mrb_value texture_height(mrb_state *mrb, mrb_value self) {
  moon_texture* texture;
  Data_Get_Struct(mrb, self, &texture_data_type, texture);

  return mrb_float_value(mrb, (*texture)->height());
}

struct RClass* mmrb_texture_init(mrb_state *mrb) {
  struct RClass *texture_class;
  texture_class = mrb_define_class_under(mrb, mmrb_Moon, "Texture", mrb->object_class);
  MRB_SET_INSTANCE_TT(texture_class, MRB_TT_DATA);

  mrb_define_method(mrb, texture_class, "initialize", texture_initialize, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, texture_class, "width",      texture_width,      MRB_ARGS_NONE());
  mrb_define_method(mrb, texture_class, "height",     texture_height,     MRB_ARGS_NONE());

  return texture_class;
};
