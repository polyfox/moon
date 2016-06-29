#ifndef MOON_AUDIO_LIBSOUNDIO_SOUND_BUFFER_H
#define MOON_AUDIO_LIBSOUNDIO_SOUND_BUFFER_H

#include "moon/intern.h"

namespace Moon {
	template<typename T> struct AbstractSoundBuffer {
		// How many channels does the buffer contain
		int channelCount;
		// in samples
		int length;
		// Array[channel][index] == sample for channel
		T** samples;

		AbstractSoundBuffer() {}

		AbstractSoundBuffer(size_t p_channels, size_t p_length)
		{
			channelCount = p_channels;
			length = p_length;
			samples = (T**)moon_malloczero(sizeof(samples) * channelCount);
			for (int i = 0; i < length; ++i) {
				samples[i] = (float*)moon_malloczero(sizeof(T) * length);
			}
		}
	};

	typedef struct AbstractSoundBuffer<float> Float32SoundBuffer;
	typedef Float32SoundBuffer SoundBuffer;
}

#endif
