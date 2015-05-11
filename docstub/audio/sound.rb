module Moon
  # Sounds are one shot audio files, they are meant to be
  # played from memory rather than streamed.
  # Use Music instead if you need an audio stream.
  class Sound
    # @param [String] filename
    # @param [String] filetype
    def initialize(filename, filetype)
    end

    # @param [Float] gain
    # @param [Float] pitch
    # @param [Float] pan
    def play(gain, pitch, pan)
    end
  end
end
