module Moon
  class Audio
    NAME = 'moon-audio-null'

    @ticks = 0

    def self.update
      @ticks += 1
    end
  end
end
