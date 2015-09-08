#include "moon/audio/libsoundio/audio.hxx"

static void Moon_AudioWrite(struct SoundIoOutStream *outstream, int frame_count_min, int frame_count_max) {
    // Do evil stuff later.
}

namespace Moon
{
  void Audio::Initialize()
  {
    m_soundIO = soundio_create();
    // Should I use assert, or set a flag and leave it for mruby to pick up?
    assert(m_soundIO);
    int err = soundio_connect(m_soundIO);
    if (err) {
        // Say bad things happened, and moon will now implode.
    }
    soundio_flush_events(m_soundIO);
    const int defaultDeviceIndex = soundio_default_output_device_index(m_soundIO);
    if (defaultDeviceIndex < 0) {
        // Now that I think about it, we could have an error field :D
    }
    m_device = soundio_get_output_device(m_soundIO, defaultDeviceIndex);
    // if m_device is null we should raise AudioError, "device not present, out of memory?"
    m_outStream = soundio_outstream_create(m_device);
    // High quality shiiit :3
    m_outStream->format = SoundIoFormatFloat32NE;
    m_outStream->writeCallback = Moon_AudioWrite;

    err = soundio_outstream_open(m_outStream);
    if (err) {
        // raise AudioError, "stream could not be opened"
    }
    if (m_outStream->layout_error) {
        // raise AudioError, "unable to set channel layout"
    }
    err = soundio_outstream_start(m_outStream);
    if (err) {
        // raise AudioError, "unable to start device"
    }
    // Finally, after bashing your head against the table, you can finally have audio?
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
