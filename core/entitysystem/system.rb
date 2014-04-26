module System

  @@system_list = {}

  def self.[](key)
    @@system_list[key]
  end

  def self.list
    @@system_list
  end

  def process(delta, world)
    #
  end

  def to_h
    {
      class: to_s
    }
  end

  def export
    to_h.stringify_keys
  end

  def import(data)
    self
  end

  def register(sym)
    @@system_list.delete(@registered) if @registered
    @registered = sym
    @@system_list[sym] = self
  end

  def self.extended(mod)
    mod.register mod.to_s.demodulize.downcase.to_sym
  end

  def self.load(data)
    Object.const_get(data["class"])
  end

end
