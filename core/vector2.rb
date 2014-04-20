#
# moon/core/vector2.rb
#   Everyone loves a Vector class
module Moon
  class Vector2

    include Comparable

    def zero?
      x == 0 && y == 0
    end

    def <=>(other)
      ox, oy = *Vector2.extract(other)
      [ox, oy] <=> [x, y]
    end

    def to_h
      { x: x, y: y }
    end

    def round(*a)
      Vector2.new x.round(*a), y.round(*a)
    end

    def floor
      Vector2.new x.floor, y.floor
    end

    def ceil
      Vector2.new x.ceil, y.ceil
    end

    def normalize
      m = [x, y].max.to_f
      Vector2.new x / m, y / m
    end

    def cross(other)
      vx, vy = *Vector2.extract(other)
      Vector2.new x * vy, y * vx
    end unless method_defined? :cross

    def rotate(r)
      Vector2.new x * Math.cos(r) - y * Math.sin(r),
                  x * Math.sin(r) + y * Math.cos(r)
    end unless method_defined? :rotate

    def mag
      Math.sqrt x * x + y * y
    end

    def rad
      Math.atan2 y, x
    end

    def rad=(n)
      m = mag
      self.x = m * Math.cos(n)
      self.y = m * Math.sin(n)
    end

    def xy
      dup
    end

    def xy=(other)
      self.x, self.y = *Vector2.extract(other)
    end

    def xyz
      Vector3.new x, y, 0
    end

    def xyz=(other)
      self.x, self.y, _ = *Vector3.extract(other)
    end

    def self.zero
      new 0.0, 0.0
    end

  end
end