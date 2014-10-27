#include "moon/mrb.hxx"
#include "moon/mrb/context.hxx"
#include "moon/mrb/font.hxx"
#include "moon/mrb/input.hxx"
#include "moon/mrb/music.hxx"
#include "moon/mrb/rect.hxx"
#include "moon/mrb/screen.hxx"
#include "moon/mrb/sound.hxx"
#include "moon/mrb/sprite.hxx"
#include "moon/mrb/spritesheet.hxx"
#include "moon/mrb/texture.hxx"
#include "moon/mrb/transform.hxx"
#include "moon/mrb/vector2.hxx"
#include "moon/mrb/vector3.hxx"
#include "moon/mrb/vector4.hxx"

struct mrb_data_type engine_data_type = { "Engine", NULL };
struct mrb_data_type window_data_type = { "Window", NULL };

struct RClass* mmrb_Moon;
/* System */
struct RClass* mmrb_Input;
struct RClass* mmrb_Screen;
/* Data */
struct RClass* mmrb_Context;
struct RClass* mmrb_Rect;
struct RClass* mmrb_Vector2;
struct RClass* mmrb_Vector3;
struct RClass* mmrb_Vector4;
struct RClass* mmrb_Transform;
/* Audio */
struct RClass* mmrb_Music;
struct RClass* mmrb_Sound;
/* Graphics */
struct RClass* mmrb_Texture;
struct RClass* mmrb_Font;
struct RClass* mmrb_Sprite;
struct RClass* mmrb_Spritesheet;

void mmrb_init(mrb_state *mrb) {
  mmrb_Moon        = mrb_define_module(mrb, "Moon");
  /* System */
  mmrb_Input       = mmrb_input_init(mrb);
  mmrb_Screen      = mmrb_screen_init(mrb);
  /* Data */
  mmrb_Context     = mmrb_context_init(mrb);
  mmrb_Rect        = mmrb_rect_init(mrb);
  mmrb_Vector2     = mmrb_vector2_init(mrb);
  mmrb_Vector3     = mmrb_vector3_init(mrb);
  mmrb_Vector4     = mmrb_vector4_init(mrb);
  mmrb_Transform   = mmrb_transform_init(mrb);
  /* Audio */
  mmrb_Music       = mmrb_music_init(mrb);
  mmrb_Sound       = mmrb_sound_init(mrb);
  /* Graphics */
  mmrb_Texture     = mmrb_texture_init(mrb);
  mmrb_Font        = mmrb_font_init(mrb);
  mmrb_Sprite      = mmrb_sprite_init(mrb);
  mmrb_Spritesheet = mmrb_spritesheet_init(mrb);
};
