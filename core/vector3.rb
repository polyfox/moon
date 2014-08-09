#
# moon/core/vector3.rb
#   Everyone loves a Vector class
module Moon
  class Vector3

    include Comparable

    def zero?
      x == 0 && y == 0 && z == 0
    end

    def <=>(other)
      ox, oy, oz = *Vector3.extract(other)
      [ox, oy, oz] <=> [x, y, z]
    end

    def to_h
      { x: x, y: y, z: z }
    end

    def export
      to_h.merge("&class" => self.class.to_s).stringify_keys
    end

    def import(data)
      self.x = data["x"]
      self.y = data["y"]
      self.z = data["z"]
      self
    end

    def round(*a)
      Vector3.new x.round(*a), y.round(*a), z.round(*a)
    end

    def floor
      Vector3.new x.floor, y.floor, z.floor
    end

    def ceil
      Vector3.new x.ceil, y.ceil, z.ceil
    end

    def abs
      Vector3.new x.abs, y.abs, z.abs
    end

    def normalize
      m = [x, y, z].max.to_f
      Vector3.new x / m, y / m, z / m
    end

    def mag
      Math.sqrt x * x + y * y + z * z
    end

    def xy
      Vector2.new x, y
    end

    def xy=(other)
      self.x, self.y = *Vector2.extract(other)
    end

    def xyz
      dup
    end

    def xyz=(other)
      self.x, self.y, self.z = *Vector3.extract(other)
    end

    def inspect
      "<Moon::Vector3: x=#{x} y=#{y} z=#{z}>"
    end

    alias :to_s :inspect

    def self.zero
      new 0.0, 0.0, 0.0
    end

    def self.load(data)
      new data["x"], data["y"], data["z"]
    end

    alias :rgb :xyz
    alias :rgb= :xyz=

  end
end
