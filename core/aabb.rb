# http://studiofreya.com/3d-math-and-physics/simple-aabb-vs-aabb-collision-detection/
module Moon
  class AABB

    attr_reader :cpos # center position
    attr_reader :rad  # radii

    def initialize(cpos, rad)
      @cpos = Vector3[cpos]
      @rad = Vector3[rad]
    end

    def intersect?(other)
      return false if (@cpos.x - other.cpos.x).abs > (@rad.x + other.rad.x)
      return false if (@cpos.y - other.cpos.y).abs > (@rad.y + other.rad.y)
      return false if (@cpos.z - other.cpos.z).abs > (@rad.z + other.rad.z)
      true
    end

  end
end