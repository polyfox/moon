module Moon # :nodoc:
  # Utility class for tracking time.
  class Clock
    def initialize
    end

    # @return [self]
    def initialize_copy
    end

    # @return [Float]
    def restart
    end

    # @return [self]
    def update
    end

    # @return [Float]
    def delta
    end

    # @return [Integer]
    def fps
    end
  end

  # Utility class for code execution, all code evaluated in a Context will be
  # retained.
  # Currently unstable, still on the TODO list.
  class Context
    # Initialies the Context
    def initialize
    end

    # Evaluates a given string in the Context
    # @param [String] str
    def eval(str)
    end

    # @yieldparam [Symbol] symbol
    def each_symbol
    end
  end

  #
  class Rect
    # @return [Integer]
    attr_accessor :x
    # @return [Integer]
    attr_accessor :y
    # @return [Integer]
    attr_accessor :w
    # @return [Integer]
    attr_accessor :h

    # @param [Integer] x
    # @param [Integer] y
    # @param [Integer] w
    # @param [Integer] h
    def initialize(x = 0, y = 0, w = 0, h = 0)
    end
  end

  #
  class Transform
    def initialize(*args)
    end

    # @return [self]
    def initialize_copy(other)
    end

    # @return [Array<Object>]
    def coerce(other)
    end

    def []
    end

    def []=
    end

    def -@
    end

    def +@
    end

    def +(other)
    end

    def -(other)
    end

    def *(other)
    end

    def /(other)
    end

    def translate(other)
    end

    def rotate(other)
    end

    def scale(other)
    end

    def to_a16
    end

    def to_a
    end

    def self.[](*args)
    end

    def self.ortho(a, b, c, d, e, f)
    end
  end

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

  #
  class Vector3
    # @return [Float]
    attr_accessor :x
    # @return [Float]
    attr_accessor :y
    # @return [Float]
    attr_accessor :z

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
