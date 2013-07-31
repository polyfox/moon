#include "moon.hxx"
#include "spritesheet.hxx"

namespace Moon {
  static void moon_mrb_spritesheet_deallocate(mrb_state *mrb, void *p) {
    delete((Spritesheet*)p);
  };

  static const struct mrb_data_type spritesheet_data_type = {
    "spritesheet", moon_mrb_spritesheet_deallocate,
  };

  static mrb_value moon_mrb_spritesheet_new(mrb_state *mrb, mrb_value klass) {
    mrb_value filename;
    mrb_int tile_width, tile_height;
    mrb_get_args(mrb, "Sii", &filename, &tile_width, &tile_height);

    Spritesheet *spritesheet = new Spritesheet(mrb_string_value_ptr(mrb, filename), tile_width, tile_height);
    return mrb_obj_value(Data_Wrap_Struct(mrb, mrb_class_ptr(klass), &spritesheet_data_type, spritesheet));
  };

  static mrb_value moon_mrb_spritesheet_render(mrb_state *mrb, mrb_value self) {
    mrb_int x, y, index;
    mrb_float z;
    mrb_get_args(mrb, "iifi", &x, &y, &z, &index);

    Spritesheet *spritesheet;
    Data_Get_Struct(mrb, self, &spritesheet_data_type, spritesheet);
    spritesheet->render(x, y, z, index);
    return mrb_nil_value();
  };

  static mrb_value moon_mrb_spritesheet_cell_width(mrb_state *mrb, mrb_value self) {
    Spritesheet *spritesheet;
    Data_Get_Struct(mrb, self, &spritesheet_data_type, spritesheet);
    return mrb_fixnum_value((int)spritesheet->tileWidth);
  }

  static mrb_value moon_mrb_spritesheet_cell_height(mrb_state *mrb, mrb_value self) {
    Spritesheet *spritesheet;
    Data_Get_Struct(mrb, self, &spritesheet_data_type, spritesheet);
    return mrb_fixnum_value((int)spritesheet->tileHeight);
  }

  void moon_mrb_spritesheet_init(mrb_state *mrb) {
    struct RClass *spritesheet_class;
    spritesheet_class = mrb_define_class(mrb, "Spritesheet", mrb->object_class);
    MRB_SET_INSTANCE_TT(spritesheet_class, MRB_TT_DATA);
    
    mrb_define_class_method(mrb, spritesheet_class, "new", moon_mrb_spritesheet_new, MRB_ARGS_REQ(3));
    mrb_define_method(mrb, spritesheet_class, "render", moon_mrb_spritesheet_render, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, spritesheet_class, "cell_width", moon_mrb_spritesheet_cell_width, MRB_ARGS_NONE());
    mrb_define_method(mrb, spritesheet_class, "cell_height", moon_mrb_spritesheet_cell_height, MRB_ARGS_NONE());
  };
};