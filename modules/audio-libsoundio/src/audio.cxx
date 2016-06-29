#include "moon/intern.h"
#include "moon/audio/libsoundio/audio.hxx"

static void Moon_AudioWrite(struct SoundIoOutStream *outstream, int frame_count_min, int frame_count_max) {
  // Do evil stuff later.
}

namespace Moon
{
  struct SoundIo* Audio::m_soundIO = NULL;
  struct SoundIoDevice* Audio::m_device = NULL;
  struct SoundIoOutStream* Audio::m_outStream = NULL;

  Audio::ErrorCode Audio::Initialize()
  {
    m_soundIO = soundio_create();
    // Should I use assert, or set a flag and leave it for mruby to pick up?
    if (!m_soundIO) {
      return Moon::Audio::ErrorCode::MOON_AUDIO_CREATE_ERROR;
    }
    int err = soundio_connect(m_soundIO);
    if (err) {
      return Moon::Audio::ErrorCode::MOON_AUDIO_CONNECTION_ERROR;
    }
    soundio_flush_events(m_soundIO);
    // Determine the default device
    const int defaultDeviceIndex = soundio_default_output_device_index(m_soundIO);
    if (defaultDeviceIndex < 0) {
      return Moon::Audio::ErrorCode::MOON_AUDIO_NO_DEVICE;
    }
    // retrieve the device specified by the index
    m_device = soundio_get_output_device(m_soundIO, defaultDeviceIndex);
    if (!m_device) {
      return Moon::Audio::ErrorCode::MOON_AUDIO_DEVICE_MISSING;
    }
    // create an output stream
    m_outStream = soundio_outstream_create(m_device);
    if (m_outStream)
    m_outStream->format = SoundIoFormatFloat32NE;
    m_outStream->write_callback = Moon_AudioWrite;

    // open the stream
    err = soundio_outstream_open(m_outStream);
    if (err) {
      return Moon::Audio::ErrorCode::MOON_AUDIO_COULD_NOT_OPEN_STREAM;
    }
    if (m_outStream->layout_error) {
      return Moon::Audio::ErrorCode::MOON_AUDIO_STREAM_CHANNEL_LAYOUT_ERROR;
    }
    err = soundio_outstream_start(m_outStream);
    if (err) {
      return Moon::Audio::ErrorCode::MOON_AUDIO_STREAM_START_ERROR;
      // raise AudioError, "unable to start device"
    }
    // Finally, after bashing your head against the table, you can finally have audio?
    return Moon::Audio::ErrorCode::MOON_AUDIO_OK;
  }

  void Audio::Update()
  {
    soundio_flush_events(m_soundIO);
  }

  void Audio::Terminate()
  {
    soundio_outstream_destroy(m_outStream);
    soundio_device_unref(m_device);
    soundio_destroy(m_soundIO);
  }
}
