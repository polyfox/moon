#
# moon/core/vector4.rb
#   Everyone loves a Vector class
module Moon
  class Vector4

    include Comparable

    def zero?
      x == 0 && y == 0 && z == 0 && w == 0
    end

    def <=>(other)
      ox, oy, oz, ow = *Vector4.extract(other)
      [ox, oy, oz, ow] <=> [x, y, z, w]
    end

    def to_h
      { x: x, y: y, z: z, w: w }
    end

    def export
      to_h.merge("&class" => self.class.to_s).stringify_keys
    end

    def import(data)
      self.x = data["x"]
      self.y = data["y"]
      self.z = data["z"]
      self.w = data["w"]
      self
    end

    def round(*a)
      Vector4.new x.round(*a), y.round(*a), z.round(*a), w.round(*a)
    end

    def floor
      Vector4.new x.floor, y.floor, z.floor, w.floor
    end

    def ceil
      Vector4.new x.ceil, y.ceil, z.ceil, w.ceil
    end

    def abs
      Vector3.new x.abs, y.abs, z.abs, w.abs
    end

    def normalize
      m = [x, y, z, w].max.to_f
      return Vector4.zero if m == 0
      Vector4.new x / m, y / m, z / m, w / m
    end

    def xy
      Vector2.new x, y
    end

    def xy=(other)
      self.x, self.y = *Vector2.extract(other)
    end

    def zw
      Vector2.new z, w
    end

    def zw=(other)
      self.z, self.w = *Vector2.extract(other)
    end

    def xyz
      Vector3.new x, y, z
    end

    def xyz=(other)
      self.x, self.y, self.z = *Vector3.extract(other)
    end

    def xyzw
      dup
    end

    def xyzw=(other)
      self.x, self.y, self.z, self.w = *Vector4.extract(other)
    end

    def inspect
      "<Moon::Vector4: x=#{x} y=#{y} z=#{z} w=#{w}>"
    end

    alias :to_s :inspect

    def self.zero
      new 0.0, 0.0, 0.0, 0.0
    end

    def self.load(data)
      new data["x"], data["y"], data["z"], data["w"]
    end

    alias :rgb :xyz
    alias :rgb= :xyz=
    alias :rgba :xyzw
    alias :rgba= :xyzw=

  end
end
