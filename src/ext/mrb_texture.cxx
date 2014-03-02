#include "mrb.hxx"
#include "texture.hxx"

namespace Moon {

  static void moon_mrb_texture_deallocate(mrb_state *mrb, void *p) {
    delete((std::shared_ptr<Texture>*)p);
  };

  const struct mrb_data_type texture_data_type = {
    "Texture", moon_mrb_texture_deallocate,
  };

  static mrb_value moon_mrb_texture_new(mrb_state *mrb, mrb_value klass) {
    char* filename;
    mrb_get_args(mrb, "z", &filename);

    // ugly hack to make a pointer to shared_ptr
    auto texture = new std::shared_ptr<Texture>(Texture::load(filename));

    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_ptr(klass), &texture_data_type, texture));
  };

  static mrb_value moon_mrb_texture_width(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Texture>* texture;
    Data_Get_Struct(mrb, self, &texture_data_type, texture);

    return mrb_float_value(mrb, (*texture)->width());
  }

  static mrb_value moon_mrb_texture_height(mrb_state *mrb, mrb_value self) {
    std::shared_ptr<Texture>* texture;
    Data_Get_Struct(mrb, self, &texture_data_type, texture);

    return mrb_float_value(mrb, (*texture)->height());
  }

  void moon_mrb_texture_init(mrb_state *mrb) {
    struct RClass *texture_class;
    texture_class = mrb_define_class_under(mrb, mrb_module_get(mrb, "Moon"), "Texture", mrb->object_class);
    MRB_SET_INSTANCE_TT(texture_class, MRB_TT_DATA);

    mrb_define_class_method(mrb, texture_class, "new", moon_mrb_texture_new, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, texture_class, "width", moon_mrb_texture_width, MRB_ARGS_NONE());
    mrb_define_method(mrb, texture_class, "height", moon_mrb_texture_height, MRB_ARGS_NONE());
  };

}