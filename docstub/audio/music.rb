module Moon
  # Object used to playing audio streams.
  class Music
    # @param [String] filename
    # @param [String] filetype
    def initialize(filename, filetype)
    end

    # @param [Float] gain
    # @param [Float] pitch
    # @param [Float] pan
    def play(gain, pitch, pan)
    end

    # Stops the playing stream
    def stop
    end

    # @param [Integer] pos
    def seek(pos)
    end

    # @return [Integer]
    def length
    end

    # @param [Integer] trigger
    # @param [Integer] target
    def loop(trigger, target)
    end

    # Cancels the currently set loop
    def clear_loop
    end

    # @return [Boolean]
    def playing?
    end

    # @return [Boolean]
    def stopped?
    end

    # @return [Boolean]
    def finished?
    end
  end
end
