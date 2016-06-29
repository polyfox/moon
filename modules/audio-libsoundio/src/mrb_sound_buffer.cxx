#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/numeric.h>
#include "moon/audio/libsoundio/mrb/sound_buffer.hxx"
#include "moon/audio/libsoundio/sound_buffer.hxx"
#include "moon/api.h"
#include "moon/intern.h"

static void
sound_buffer_free(mrb_state* mrb, void* ptr)
{
	Moon::SoundBuffer* buf = (Moon::SoundBuffer*)ptr;
	if (buf) {
		delete buf;
	}
}

MOON_C_API const struct mrb_data_type sound_buffer_data_type = { "Moon::SoundBuffer", sound_buffer_free };

MOON_C_API
Moon::SoundBuffer* mmrb_to_sound_buffer(mrb_state *mrb, mrb_value self)
{
	return static_cast<Moon::SoundBuffer*>(mrb_data_check_get_ptr(mrb, self, &sound_buffer_data_type));
}

static mrb_value
sound_buffer_initialize_by_filename(mrb_state *mrb, mrb_value self)
{
	char* str = NULL;
	mrb_get_args(mrb, "z", &str);
	mrb_raise(mrb, E_RUNTIME_ERROR, "not implemented");
	return self;
}

static mrb_value
sound_buffer_initialize_create(mrb_state *mrb, mrb_value self)
{
	mrb_int channels = 0;
	mrb_int length = 0;
	mrb_get_args(mrb, "ii", &channels, &length);
	if (channels <= 0) {
		mrb_raise(mrb, E_ARGUMENT_ERROR, "channels must be 1 or more");
		return self;
	}
	if (length <= 0) {
		mrb_raise(mrb, E_ARGUMENT_ERROR, "length must be 1 or more");
		return self;
	}
	Moon::SoundBuffer *buffer = new Moon::SoundBuffer(channels, length);
	mrb_data_init(self, buffer, &sound_buffer_data_type);
	return self;
}

static mrb_value
sound_buffer_aget(mrb_state *mrb, mrb_value self)
{
	mrb_int channel = 0;
	mrb_int index = 0;
	mrb_get_args(mrb, "ii", &channel, &index);
	Moon::SoundBuffer* buf = mmrb_to_sound_buffer(mrb, self);
	if (channel < 0 || channel >= buf->channelCount) {
		mrb_raise(mrb, E_ARGUMENT_ERROR, "channel index exceeded!");
		return mrb_float_value(mrb, 0.0f);
	}

	if (index < 0) {
		index %= buf->length;
	}

	if (index >= buf->length) {
		mrb_raise(mrb, E_ARGUMENT_ERROR, "buffer length exceeded!");
		return mrb_float_value(mrb, 0.0f);
	}

	return mrb_float_value(mrb, buf->samples[channel][index]);
}

static mrb_value
sound_buffer_aset(mrb_state *mrb, mrb_value self)
{
	mrb_int channel = 0;
	mrb_int index = 0;
	mrb_float value = 0.0f;
	mrb_get_args(mrb, "iif", &channel, &index, &value);
	Moon::SoundBuffer* buf = mmrb_to_sound_buffer(mrb, self);
	if (channel < 0 || channel >= buf->channelCount) {
		mrb_raise(mrb, E_ARGUMENT_ERROR, "channel index exceeded!");
		return mrb_float_value(mrb, 0.0f);
	}

	if (index < 0) {
		index %= buf->length;
	}

	if (index >= buf->length) {
		mrb_raise(mrb, E_ARGUMENT_ERROR, "buffer length exceeded!");
		return mrb_float_value(mrb, 0.0f);
	}

	buf->samples[channel][index] = (float)value;
	return self;
}

MOON_C_API void
mmrb_sound_buffer_init(mrb_state *mrb, struct RClass *mod)
{
	struct RClass* sound_buffer_class = mrb_define_class_under(mrb, mod, "SoundBuffer", mrb->object_class);

	mrb_define_method(mrb, sound_buffer_class, "initialize_by_filename", sound_buffer_initialize_by_filename, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, sound_buffer_class, "initialize_create", sound_buffer_initialize_create, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, sound_buffer_class, "[]", sound_buffer_aget, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, sound_buffer_class, "[]=", sound_buffer_aset, MRB_ARGS_REQ(3));
}
