module Moon
  class Transform

    def to_h
      {
        data: to_a
      }
    end

    def export
      to_h.merge("&class" => self.class.to_s).stringify_keys
    end

    def import(data)
      dat = data["data"]
      self[0] = dat[0, 4]
      self[1] = dat[4, 4]
      self[2] = dat[8, 4]
      self[3] = dat[12, 4]
      self
    end

    def self.load(data)
      new(*data["data"])
    end

    def self.translate(*args)
      new.translate(*args)
    end

    def self.rotate(*args)
      new.rotate(*args)
    end

    def self.scale(*args)
      new.scale(*args)
    end

  end
end