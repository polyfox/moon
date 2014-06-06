module Moon
  class Event

    @@id = 0

    attr_reader :type
    attr_reader :id

    def initialize(type)
      @type = type
      @id = @@id += 1
    end

  end
  class InputEvent < Event

    attr_reader :action, :key, :mods

    def initialize(key, action, mods)
      @action = action
      @key = key

      @mods = mods
      super :input
    end

    def alt?
      @mods.masked? Moon::Input::MOD_ALT
    end

    def ctrl?
      @mods.masked? Moon::Input::MOD_CONTROL
    end

    def super?
      @mods.masked? Moon::Input::MOD_SUPER
    end

    def shift?
      @mods.masked? Moon::Input::MOD_SHIFT
    end

  end
end
