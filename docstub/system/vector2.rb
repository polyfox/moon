module Moon
  #
  class Vector2
    attr_accessor :x
    attr_accessor :y

    def initialize
    end

    # @return [self]
    def initialize_copy(other)
    end

    # @overload set(v2)
    #   @param [Vector2, #to_vec2] v2
    # @overload set(x, y)
    #   @param [Vector1, Numeric] x
    #   @param [Vector1, Numeric] y
    def set(*args)
    end

    # @return [Vector2]
    def -@
    end

    # @return [Vector2]
    def +@
    end

    # @return [Vector2]
    def +(other)
    end

    # @return [Vector2]
    def -(other)
    end

    # @return [Vector2]
    def *(other)
    end

    # @return [Vector2]
    def /(other)
    end

    # @return [Vector2]
    def dot(other)
    end

    # @return [Vector1]
    def normalize
    end

    # @return [Float]
    def length
    end

    # @param [Vector2] other
    # @return [Float]
    def distance(other)
    end

    # @param [Vector2] other
    # @param [Float] delta
    def lerp(other, delta)
    end

    # @return [Array<Float>]
    def to_a
    end

    def self.[](*args)
    end

    # @return [Array<Float>]
    def self.extract(obj)
    end
  end
end
