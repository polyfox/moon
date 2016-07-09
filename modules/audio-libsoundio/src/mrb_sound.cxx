#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/audio/libsoundio/mrb/sound.hxx"
#include "moon/audio/libsoundio/sound.hxx"
#include "moon/api.h"
#include "moon/intern.h"
#include "moon/mrb/helpers.hxx"

static void
sound_free(mrb_state* mrb, void *p)
{
	Moon::Sound *sound = static_cast<Moon::Sound*>(p);
	if (sound) {
		delete(sound);
	}
}

MOON_C_API const struct mrb_data_type sound_data_type = { "Moon::Sound", sound_free };

static mrb_value
sound_initialize(mrb_state* mrb, mrb_value self)
{
	char* filename;
	mrb_get_args(mrb, "z", &filename);
	moon_data_cleanup(mrb, self);
	Moon::Sound* sound = new Moon::Sound(std::string(filename));
	mrb_data_init(self, sound, &sound_data_type);
	return self;
}

static mrb_value
sound_play(mrb_state* mrb, mrb_value self)
{
	mrb_float gain = 1.0;
	mrb_float pitch = 1.0;
	mrb_float pan = 0.0;
	mrb_get_args(mrb, "|fff", &gain, &pitch, &pan);
	// TODO
	return self;
}

MOON_C_API void
mmrb_sound_init(mrb_state* mrb, struct RClass *mod)
{
	struct RClass *sound_class = mrb_define_class_under(mrb, mod, "Sound", mrb->object_class);
	MRB_SET_INSTANCE_TT(sound_class, MRB_TT_DATA);

	mrb_define_method(mrb, sound_class, "initialize", sound_initialize, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, sound_class, "play", sound_play,             MRB_ARGS_OPT(3));
}
