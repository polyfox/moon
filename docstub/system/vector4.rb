module Moon
  #
  class Vector4
    # @return [Float]
    attr_accessor :x
    # @return [Float]
    attr_accessor :y
    # @return [Float]
    attr_accessor :z
    # @return [Float]
    attr_accessor :w

    def initialize
    end

    # @return [self]
    def initialize_copy(other)
    end

    # @return [self]
    def set(*args)
    end

    # @return [Vector4]
    def -@
    end

    # @return [Vector4]
    def +@
    end

    # @return [Array<Float>]
    def self.extract(obj)
    end
  end
end
