module System

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

  def self.load(data)
    Object.const_get(data["class"])
  end

end
