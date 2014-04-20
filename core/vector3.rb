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

    def round(*a)
      Vector3.new x.round(*a), y.round(*a), z.round(*a)
    end

    def floor
      Vector3.new x.floor, y.floor, z.floor
    end

    def ceil
      Vector3.new x.ceil, y.ceil, z.ceil
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

    def self.zero
      new 0.0, 0.0, 0.0
    end

  end
end