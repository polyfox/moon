#ifndef MOON_AUDIO_LIBSOUNDIO_SOUND_BUFFER_H
#define MOON_AUDIO_LIBSOUNDIO_SOUND_BUFFER_H

#include "moon/intern.h"

namespace Moon {
	template<typename T> struct AbstractSoundBuffer {
		// How many channels does the buffer contain
		size_t channelCount;
		// sample rate 44100
		int sampleRate;
		// in samples
		size_t length;
		// Array[channel][index] == sample for channel
		T** samples;

		static
		AbstractSoundBuffer<T>* create(size_t p_channels, size_t p_length, int p_sampleRate)
		{
			AbstractSoundBuffer<T>* buffer = new AbstractSoundBuffer<T>();
			buffer->channelCount = p_channels;
			buffer->length = p_length;
			buffer->sampleRate = p_sampleRate;
			buffer->samples = (T**)moon_malloczero(sizeof(buffer->samples) * buffer->channelCount);
			assert(buffer->samples);
			for (int i = 0; i < buffer->length; ++i) {
				buffer->samples[i] = (float*)moon_malloczero(sizeof(T) * buffer->length);
			}
		}
	};

	typedef struct AbstractSoundBuffer<float> Float32SoundBuffer;
	typedef Float32SoundBuffer SoundBuffer;
}

#endif
