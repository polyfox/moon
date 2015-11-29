module Moon
  class Music
    def initialize(filename, format)
      @filename = filename
      @format = format
    end

    def play(gain = 1.0, pitch = 1.0, pan = 0.0)
    end

    def stop
    end

    def pos
      0
    end

    def length
      0
    end

    def loop(trigger, target)
    end

    def clear_loop
    end

    def playing?
      false
    end

    def stopped?
      true
    end

    def finished?
      false
    end
  end
end
