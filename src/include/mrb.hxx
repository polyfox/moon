#ifndef MOON_MRB_H
#define MOON_MRB_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/compile.h>
#include <mruby/variable.h>
#include <mruby/string.h>

namespace Moon {
  extern const struct mrb_data_type font_data_type;
  extern const struct mrb_data_type sound_data_type;
  extern const struct mrb_data_type sprite_data_type;
  extern const struct mrb_data_type spritesheet_data_type;
  extern const struct mrb_data_type tone_data_type;

  void moon_mrb_font_init(mrb_state *mrb);
  void moon_mrb_input_init(mrb_state *mrb);
  void moon_mrb_sprite_init(mrb_state *mrb);
  void moon_mrb_spritesheet_init(mrb_state *mrb);
  void moon_mrb_sound_init(mrb_state *mrb);
  void moon_mrb_music_init(mrb_state *mrb);
  void moon_mrb_tone_init(mrb_state *mrb);
  void moon_mrb_screen_init(mrb_state *mrb);
};

void moon_init_mrb_core(mrb_state *mrb);
void moon_init_mrb_ext(mrb_state *mrb);

#endif