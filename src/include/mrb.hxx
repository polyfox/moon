#ifndef MOON_MRB_H
#define MOON_MRB_H

#include <mruby.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/compile.h>
#include <mruby/data.h>
#include <mruby/hash.h>
#include <mruby/numeric.h>
#include <mruby/proc.h>
#include <mruby/string.h>
#include <mruby/variable.h>

namespace Moon {

  extern const struct mrb_data_type font_data_type;
  extern const struct mrb_data_type rect_data_type;
  extern const struct mrb_data_type sound_data_type;
  extern const struct mrb_data_type sprite_data_type;
  extern const struct mrb_data_type spritesheet_data_type;
  extern const struct mrb_data_type texture_data_type;
  extern const struct mrb_data_type tone_data_type;
  extern const struct mrb_data_type vector2_data_type;
  extern const struct mrb_data_type vector3_data_type;
  extern const struct mrb_data_type vector4_data_type;
  namespace mrb_Transform { extern const struct mrb_data_type data_type; }
  extern const struct mrb_data_type context_data_type;
  extern const struct mrb_data_type engine_data_type;
  extern const struct mrb_data_type window_data_type;

  extern struct RClass *moon_module;
  extern struct RClass *moon_cFont;
  extern struct RClass *moon_mInput;
  extern struct RClass *moon_cSprite;
  extern struct RClass *moon_cSpritesheet;
  extern struct RClass *moon_cSound;
  extern struct RClass *moon_cMusic;
  extern struct RClass *moon_cRect;
  extern struct RClass *moon_cTexture;
  extern struct RClass *moon_cTone;
  extern struct RClass *moon_cScreen;
  extern struct RClass *moon_cVector2;
  extern struct RClass *moon_cVector3;
  extern struct RClass *moon_cVector4;
  extern struct RClass *moon_cTransform;
  extern struct RClass *moon_cContext;

  struct RClass* moon_mrb_font_init(mrb_state *mrb);
  struct RClass* moon_mrb_input_init(mrb_state *mrb);
  struct RClass* moon_mrb_sprite_init(mrb_state *mrb);
  struct RClass* moon_mrb_spritesheet_init(mrb_state *mrb);
  struct RClass* moon_mrb_sound_init(mrb_state *mrb);
  struct RClass* moon_mrb_music_init(mrb_state *mrb);
  struct RClass* moon_mrb_rect_init(mrb_state *mrb);
  struct RClass* moon_mrb_texture_init(mrb_state *mrb);
  struct RClass* moon_mrb_tone_init(mrb_state *mrb);
  struct RClass* moon_mrb_screen_init(mrb_state *mrb);
  struct RClass* moon_mrb_vector2_init(mrb_state *mrb);
  struct RClass* moon_mrb_vector3_init(mrb_state *mrb);
  struct RClass* moon_mrb_vector4_init(mrb_state *mrb);
  struct RClass* moon_mrb_transform_init(mrb_state *mrb);
  struct RClass* moon_mrb_context_init(mrb_state *mrb);
};

void moon_init_mrb_core(mrb_state *mrb);
void moon_init_mrb_ext(mrb_state *mrb);

#endif