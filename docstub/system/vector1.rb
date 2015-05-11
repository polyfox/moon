module Moon
  #
  class Vector1
    # @return [Float]
    attr_accessor :x

    # @param [Float] x
    def initialize(x)
    end

    # @return [self]
    def initialize_copy(other)
    end

    # @param [Vector1, Numeric] obj
    def set(obj)
    end

    # @return [Vector1]
    def -@
    end

    # @return [Vector1]
    def +@
    end

    # @return [Vector1]
    def +(other)
    end

    # @return [Vector1]
    def -(other)
    end

    # @return [Vector1]
    def *(other)
    end

    # @return [Vector1]
    def /(other)
    end

    # @return [Vector1]
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

    # @return [Array<Float>]
    def self.extract(obj)
    end
  end
end
