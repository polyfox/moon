#include "mrb.hxx"

void moon_init_mrb_core(mrb_state *mrb) {
  Moon::moon_mrb_font_init(mrb);
  Moon::moon_mrb_input_init(mrb);
  Moon::moon_mrb_sprite_init(mrb);
  Moon::moon_mrb_spritesheet_init(mrb);
  Moon::moon_mrb_sound_init(mrb);
  Moon::moon_mrb_music_init(mrb);
  Moon::moon_mrb_tone_init(mrb);
  Moon::moon_mrb_window_init(mrb);
};