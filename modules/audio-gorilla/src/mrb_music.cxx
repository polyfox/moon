#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/gorilla/mrb/music.hxx"
#include "moon/gorilla/music.hxx"
#include "moon/api.h"
#include "moon/intern.h"

static void
music_free(mrb_state *mrb, void *p)
{
  Moon::Music* music = (Moon::Music*)p;
  if (music) {
    if (music->handle) {
      ga_handle_destroy(music->handle);
    }
    delete(music);
  }
}

MOON_C_API const struct mrb_data_type music_data_type = { "Music", music_free };

static inline Moon::Music*
get_music(mrb_state *mrb, mrb_value self)
{
  return (Moon::Music*)mrb_data_get_ptr(mrb, self, &music_data_type);
}

/* 
 * @param [String] filename path to the music file
 * @param [String] format type of the file
 * @return [Music]
 */
static mrb_value
music_initialize(mrb_state *mrb, mrb_value self)
{
  char *filename;
  char *format;
  Moon::Music *music;

  mrb_get_args(mrb, "zz", &filename, &format);

  music = (Moon::Music*)DATA_PTR(self);
  if (music) {
    music_free(mrb, (void*)music);
  }
  music = new Moon::Music();

  if (exists(filename)) {
    music->handle = gau_create_handle_buffered_file(Moon::Audio::GetMixer(), Moon::Audio::GetStreamMgr(),
                                                    filename, format,
                                                    NULL, 0, &(music->loopSrc));
  } else {
    mrb_raisef(mrb, E_SCRIPT_ERROR, "cannot load such file -- %S", mrb_str_new_cstr(mrb, filename));
  }

  mrb_data_init(self, music, &music_data_type);

  return self;
}

/* Plays the file.
 *
 * @param [Float] gain
 * @param [Float] pitch
 * @param [Float] pan
 * @return [nil]
 */
static mrb_value
music_play(mrb_state *mrb, mrb_value self)
{
  mrb_float gain = 1.0;
  mrb_float pitch = 1.0;
  mrb_float pan = 0.0;
  Moon::Music *music = get_music(mrb, self);
  mrb_get_args(mrb, "|fff", &gain, &pitch, &pan);
  ga_handle_setParamf(music->handle, GA_HANDLE_PARAM_GAIN, gain);
  ga_handle_setParamf(music->handle, GA_HANDLE_PARAM_PITCH, pitch);
  ga_handle_setParamf(music->handle, GA_HANDLE_PARAM_PAN, pan);
  ga_handle_play(music->handle);

  return mrb_nil_value();
};

/* Stops the playback.
 * @return [nil]
 */
static mrb_value
music_stop(mrb_state *mrb, mrb_value self)
{
  Moon::Music *music = get_music(mrb, self);
  ga_handle_stop(music->handle);
  return mrb_nil_value();
};

/* Checks if the file is currently playing.
 *
 * @return [Boolean] Returns true if currently playing, else false.
 */
static mrb_value
music_is_playing(mrb_state *mrb, mrb_value self)
{
  Moon::Music *music = get_music(mrb, self);
  return mrb_bool_value(ga_handle_playing(music->handle));
}

/* Checks if the file is currently stopped.
 *
 * @return [Boolean] Returns true if currently stopped, else false.
 */
static mrb_value
music_is_stopped(mrb_state *mrb, mrb_value self)
{
  Moon::Music *music = get_music(mrb, self);
  return mrb_bool_value(ga_handle_stopped(music->handle));
}

/* Checks if the playback is finished.
 *
 * @return [Boolean] Returns true if the playback is finished, else false.
 */
static mrb_value
music_is_finished(mrb_state *mrb, mrb_value self)
{
  Moon::Music *music = get_music(mrb, self);
  return mrb_bool_value(ga_handle_finished(music->handle));
}

/* Seeks to a certain time in the file.
 *
 * @param [Integer] pos Offset from the start of the file, in seconds.
 * @return [Boolean] Returns true if the seek was successful, else false.
 */
static mrb_value
music_seek(mrb_state *mrb, mrb_value self)
{
  mrb_int pos;
  Moon::Music *music = get_music(mrb, self);
  mrb_get_args(mrb, "i", &pos);
  return mrb_bool_value(ga_handle_seek(music->handle, pos));
}

/* Returns the current position in the file.
 *
 * @return [Integer] Current position in the file (in seconds).
 */
static mrb_value
music_pos(mrb_state *mrb, mrb_value self)
{
  Moon::Music *music = get_music(mrb, self);
  return mrb_fixnum_value(ga_handle_tell(music->handle, GA_TELL_PARAM_CURRENT));
}

/* Returns the total time length of the file.
 *
 * @return [Integer] Total length (in seconds).
 */
static mrb_value
music_length(mrb_state *mrb, mrb_value self)
{
  Moon::Music *music = get_music(mrb, self);
  return mrb_fixnum_value(ga_handle_tell(music->handle, GA_TELL_PARAM_TOTAL));
}

/* Set a loop that starts at +trigger+ sample and jumps to +target+ sample.
 *
 * @param [Integer] trigger position (in samples) where to start the jump
 * @param [Integer] target position (in samples) where to jump to
 * @return [nil]
 */
static mrb_value
music_loop(mrb_state *mrb, mrb_value self)
{
  mrb_int trigger = -1;
  mrb_int target = 0;
  Moon::Music *music = get_music(mrb, self);
  mrb_get_args(mrb, "|ii", &trigger, &target);
  gau_sample_source_loop_set(music->loopSrc, trigger, target);
  return mrb_nil_value();
}

/* Clear the loop.
 *
 * @return [Boolean] true
 */
static mrb_value
music_clear_loop(mrb_state *mrb, mrb_value self)
{
  Moon::Music *music = get_music(mrb, self);
  gau_sample_source_loop_clear(music->loopSrc);
  return mrb_bool_value(true);
}

MOON_C_API void
mmrb_music_init(mrb_state *mrb)
{
  struct RClass* mod = mrb_define_module(mrb, "Moon");
  /* Music used to playing audio streams (usually background music). The file is
   * buffered from disk instead of streamed from memory, because songs can be
   * rather large in file size.
  */
  struct RClass *music_class = mrb_define_class_under(mrb, mod, "Music", mrb->object_class);
  MRB_SET_INSTANCE_TT(music_class, MRB_TT_DATA);

  mrb_define_method(mrb, music_class, "initialize", music_initialize, MRB_ARGS_REQ(2));

  /* Playback */
  mrb_define_method(mrb, music_class, "play", music_play, MRB_ARGS_OPT(3));
  mrb_define_method(mrb, music_class, "stop", music_stop, MRB_ARGS_NONE());

  /* Position */
  mrb_define_method(mrb, music_class, "seek",       music_seek,       MRB_ARGS_REQ(1));
  mrb_define_method(mrb, music_class, "pos",        music_pos,        MRB_ARGS_NONE());
  mrb_define_method(mrb, music_class, "length",     music_length,     MRB_ARGS_NONE());
  mrb_define_method(mrb, music_class, "loop",       music_loop,       MRB_ARGS_OPT(2));
  mrb_define_method(mrb, music_class, "clear_loop", music_clear_loop, MRB_ARGS_NONE());

  /* Query */
  mrb_define_method(mrb, music_class, "playing?",  music_is_playing,  MRB_ARGS_NONE());
  mrb_define_method(mrb, music_class, "stopped?",  music_is_stopped,  MRB_ARGS_NONE());
  mrb_define_method(mrb, music_class, "finished?", music_is_finished, MRB_ARGS_NONE());
}
