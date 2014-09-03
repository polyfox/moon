#include "mrb.hxx"
#include "texture.hxx"
#include "mrb_shared_types.hxx"

namespace Moon {
  static void moon_mrb_texture_deallocate(mrb_state *mrb, void *p) {
    delete((moon_texture*)p);
  };

  const struct mrb_data_type texture_data_type = {
    "Texture", moon_mrb_texture_deallocate,
  };

  static mrb_value
  moon_mrb_texture_initialize(mrb_state *mrb, mrb_value self) {
    char* filename;
    mrb_get_args(mrb, "z", &filename);

    // ugly hack to make a pointer to shared_ptr
    auto texture = new moon_texture(Texture::load(filename));

    DATA_TYPE(self) = &texture_data_type;
    DATA_PTR(self) = texture;

    return mrb_nil_value();
  };

  static mrb_value
  moon_mrb_texture_width(mrb_state *mrb, mrb_value self) {
    moon_texture* texture;
    Data_Get_Struct(mrb, self, &texture_data_type, texture);

    return mrb_float_value(mrb, (*texture)->width());
  }

  static mrb_value
  moon_mrb_texture_height(mrb_state *mrb, mrb_value self) {
    moon_texture* texture;
    Data_Get_Struct(mrb, self, &texture_data_type, texture);

    return mrb_float_value(mrb, (*texture)->height());
  }

  struct RClass*
  moon_mrb_texture_init(mrb_state *mrb) {
    struct RClass *texture_class;
    texture_class = mrb_define_class_under(mrb, moon_module, "Texture", mrb->object_class);
    MRB_SET_INSTANCE_TT(texture_class, MRB_TT_DATA);

    mrb_define_method(mrb, texture_class, "initialize", moon_mrb_texture_initialize, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, texture_class, "width",      moon_mrb_texture_width,      MRB_ARGS_NONE());
    mrb_define_method(mrb, texture_class, "height",     moon_mrb_texture_height,     MRB_ARGS_NONE());

    return texture_class;
  };
}
