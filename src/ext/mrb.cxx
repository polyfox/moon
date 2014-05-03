#include "mrb.hxx"

namespace Moon {
  struct RClass *moon_module;
  struct RClass *moon_cColor;
  struct RClass *moon_cFont;
  struct RClass *moon_mInput;
  struct RClass *moon_cSprite;
  struct RClass *moon_cSpritesheet;
  struct RClass *moon_cSound;
  struct RClass *moon_cMusic;
  struct RClass *moon_cRect;
  struct RClass *moon_cTexture;
  struct RClass *moon_cScreen;
  struct RClass *moon_cVector2;
  struct RClass *moon_cVector3;

  const struct mrb_data_type engine_data_type = {
    "Engine", NULL,
  };

  const struct mrb_data_type window_data_type = {
    "Window", NULL,
  };
}

void moon_init_mrb_core(mrb_state *mrb) {
  Moon::moon_module       = mrb_define_module(mrb, "Moon");
  Moon::moon_cColor       = Moon::moon_mrb_color_init(mrb);
  Moon::moon_cFont        = Moon::moon_mrb_font_init(mrb);
  Moon::moon_mInput       = Moon::moon_mrb_input_init(mrb);
  Moon::moon_cSprite      = Moon::moon_mrb_sprite_init(mrb);
  Moon::moon_cSpritesheet = Moon::moon_mrb_spritesheet_init(mrb);
  Moon::moon_cSound       = Moon::moon_mrb_sound_init(mrb);
  Moon::moon_cMusic       = Moon::moon_mrb_music_init(mrb);
  Moon::moon_cRect        = Moon::moon_mrb_rect_init(mrb);
  Moon::moon_cTexture     = Moon::moon_mrb_texture_init(mrb);
  Moon::moon_cScreen      = Moon::moon_mrb_screen_init(mrb);
  Moon::moon_cVector2     = Moon::moon_mrb_vector2_init(mrb);
  Moon::moon_cVector3     = Moon::moon_mrb_vector3_init(mrb);
};