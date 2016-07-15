#ifndef MMRB_MUSIC_H
#define MMRB_MUSIC_H

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include "moon/api.h"
#include "moon/audio/libsoundio/music.hxx"

MOON_C_API const struct mrb_data_type music_data_type;
MOON_C_API void mmrb_music_init(mrb_state *mrb, struct RClass *mod);

struct mmrb_Music
{
	Moon::Music* music;
	Moon::Handle* handle;

	mmrb_Music(Moon::Music* ptr)
	{
		music = ptr;
		handle = new Moon::Handle(music);
	}

	~mmrb_Music()
	{
		delete handle;
	}
};

#endif
