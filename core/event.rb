module Moon
  class Event

    attr_reader :type
    attr_accessor :id # TEMP, proper initialization later

    def initialize(type)
      @type = type
    end

  end
  class InputEvent < Event

    attr_reader :action, :key, :mods

    def initialize(key, action, mods)
      @action = action
      @key = key

      @mods = mods # TODO
      super :input
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
