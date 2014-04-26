module Moon
  class Event

    attr_reader :type, :key
    attr_accessor :id # TEMP, proper initialization later

    def initialize(key, type, mods)
      @type = type
      @key = key

      @mods = mods # TODO
    end

    def alt?
      @mods.masked?(Moon::Input::MOD_ALT)
    end

    def ctrl?
      @mods.masked?(Moon::Input::MOD_CONTROL)
    end

    def super?
      @mods.masked?(Moon::Input::MOD_SUPER)
    end

    def shift?
      @mods.masked?(Moon::Input::MOD_SHIFT)
    end

  end
end
