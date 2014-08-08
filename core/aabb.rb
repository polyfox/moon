# http://studiofreya.com/3d-math-and-physics/simple-aabb-vs-aabb-collision-detection/
module Moon
  class AABB
    attr_reader :cpos # Vector2  center position
    attr_reader :rad  # Vector2  radii

    def initialize(cpos, rad)
      @cpos = Vector2[cpos]
      @rad = Vector2[rad]
    end

    def intersect?(other)
      return false if (@cpos.x - other.cpos.x).abs > (@rad.x + other.rad.x)
      return false if (@cpos.y - other.cpos.y).abs > (@rad.y + other.rad.y)
      true
    end

    def &(other)
      rx = (@cpos.x - other.cpos.x)
      ry = (@cpos.y - other.cpos.y)
      self.class.new(Vector2.new(@cpos.x + rx/2, @cpos.y + ry/2),
                     Vector2.new(rx, ry))
    end
  end
end
