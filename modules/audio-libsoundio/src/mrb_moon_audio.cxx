#include <mruby.h>
#include <mruby/class.h>
#include "moon/audio/libsoundio/audio.hxx"
#include "moon/audio/libsoundio/mrb/music.hxx"
#include "moon/audio/libsoundio/mrb/sound.hxx"

static mrb_value
audio_update(mrb_state *mrb, mrb_value klass)
{
	Moon::Audio::Update();
	return klass;
}

MOON_C_API void
mrb_mruby_moon_audio_libsoundio_gem_init(mrb_state* mrb)
{
	struct RClass *moon_module = mrb_define_module(mrb, "Moon");
	struct RClass *audio_module = mrb_define_module_under(mrb, moon_module, "Audio");
	struct RClass *audio_error = mrb_define_class_under(mrb, moon_module, "AudioError", E_RUNTIME_ERROR);
	mrb_define_class_method(mrb, audio_module, "update", audio_update, MRB_ARGS_NONE());
	//mmrb_sound_buffer_init(mrb, moon_module);
	mmrb_music_init(mrb, moon_module);
	mmrb_sound_init(mrb, moon_module);

	const Moon::Audio::ErrorCode err = Moon::Audio::Initialize();
	switch (err) {
		case Moon::Audio::ErrorCode::MOON_AUDIO_CREATE_ERROR:
			mrb_raise(mrb, audio_error, "Underlying backend could not be created");
			break;
		case Moon::Audio::ErrorCode::MOON_AUDIO_CONNECTION_ERROR:
			mrb_raise(mrb, audio_error, "Could not estabish connection to backend");
			break;
		case Moon::Audio::ErrorCode::MOON_AUDIO_NO_DEVICE:
			mrb_raise(mrb, audio_error, "No default device present");
			break;
		case Moon::Audio::ErrorCode::MOON_AUDIO_DEVICE_MISSING:
			mrb_raise(mrb, audio_error, "Default device is missing");
			break;
		case Moon::Audio::ErrorCode::MOON_AUDIO_COULD_NOT_OPEN_STREAM:
			mrb_raise(mrb, audio_error, "Output stream could not be opened, does the selected device support it?");
			break;
		case Moon::Audio::ErrorCode::MOON_AUDIO_STREAM_CHANNEL_LAYOUT_ERROR:
			mrb_raise(mrb, audio_error, "Channel layout is invalid for the stream");
			break;
		case Moon::Audio::ErrorCode::MOON_AUDIO_STREAM_START_ERROR:
			mrb_raise(mrb, audio_error, "Stream could not be started");
			break;
		default:
			break;
	}
}

MOON_C_API void
mrb_mruby_moon_audio_libsoundio_gem_final(mrb_state* mrb)
{
	Moon::Audio::Terminate();
}
