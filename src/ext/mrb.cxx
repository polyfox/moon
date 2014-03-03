#include "mrb.hxx"

namespace Moon {
  struct RClass *moon_module;
  const struct mrb_data_type engine_data_type = {
    "Engine", NULL,
  };
  const struct mrb_data_type window_data_type = {
    "Window", NULL,
  };
}

void moon_init_mrb_core(mrb_state *mrb) {
  Moon::moon_module = mrb_define_module(mrb, "Moon");

  Moon::moon_mrb_color_init(mrb);
  Moon::moon_mrb_font_init(mrb);
  Moon::moon_mrb_input_init(mrb);
  Moon::moon_mrb_sprite_init(mrb);
  Moon::moon_mrb_spritesheet_init(mrb);
  Moon::moon_mrb_sound_init(mrb);
  Moon::moon_mrb_music_init(mrb);
  Moon::moon_mrb_texture_init(mrb);
  Moon::moon_mrb_tone_init(mrb);
  Moon::moon_mrb_screen_init(mrb);
};