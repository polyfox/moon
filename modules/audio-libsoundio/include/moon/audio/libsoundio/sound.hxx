#ifndef MOON_AUDIO_LIBSOUNDIO_SOUND_H
#define MOON_AUDIO_LIBSOUNDIO_SOUND_H

#include "moon/audio/libsoundio/sound_buffer.hxx"

namespace Moon {
	struct Sound {
		Moon::SoundBuffer* buffer;
	};
}

#endif
