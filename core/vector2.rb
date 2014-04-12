#
# moon/core/vector2.rb
#   Everyone loves a Vector class
class Vector2

  include Comparable

  attr_accessor :x
  attr_accessor :y

  def initialize(x=0.0, y=0.0)
    @x = x
    @y = y
  end

  def <=>(other)
    ox, oy = *Vector2.obj_to_vec2_a(other)
    [ox, oy] <=> [@x, @y]
  end

  def -@
    Vector2.new(-@x, -@y)
  end

  def +@
    dup
  end

  def coerce(a)
    return self, a
  end

  def to_a
    return @x, @y
  end

  def to_h
    { x: @x, y: @y }
  end

  def round(*a)
    @x = @x.round(*a)
    @y = @y.round(*a)
    self
  end

  def floor
    @x = @x.floor
    @y = @y.floor
    self
  end

  def ceil
    @x = @x.ceil
    @y = @y.ceil
    self
  end

  def normalize
    m = [@x, @y].max.to_f
    Vector2.new @x / m, @y / m
  end

  def set(*args)
    @x, @y = *Vector2.obj_to_vec2_a(args.size > 1 ? args : args.first)
    self
  end

  def +(other)
    x, y = *Vector2.obj_to_vec2_a(other)
    Vector2.new @x + x, @y + y
  end

  def -(other)
    x, y = *Vector2.obj_to_vec2_a(other)
    Vector2.new @x - x, @y - y
  end

  def *(other)
    x, y = *Vector2.obj_to_vec2_a(other)
    Vector2.new @x * x, @y * y
  end

  def /(other)
    x, y = *Vector2.obj_to_vec2_a(other)
    Vector2.new @x / x, @y / y
  end

  def dot(other)
    x, y = *Vector2.obj_to_vec2_a(other)
    @x * x + @y * y
  end

  #def cross(other)
  #  x, y = *obj_to_vec3a other
  #  @x * x + @y * y + @z * z
  #end

  def xy
    dup
  end

  def xy=(other)
    @x, @y = *Vector2.obj_to_vec2_a(other)
  end

  def xyz
    Vector3.new @x, @y, 0
  end

  def xyz=(other)
    @x, @y, @z = *Vector3.obj_to_vec3_a(other)
  end

  ###
  # Converts a given Object to Vector2 array
  # @param [Object]
  # @return [Array<Numeric>] (x, y)
  ###
  def self.obj_to_vec2_a(obj)
    case obj
    when Vector2 then return *obj
    when Numeric then return obj, obj
    when Array
      if obj.size != 2
        raise ArgumentError,
              "wrong Array size #{obj.size} (expected Array of size 2)"
      end
      return *obj
    else
      raise TypeError,
            "wrong argument type #{obj.class} (expected Array, Numeric or Vector2)"
    end
  end

  def self.[](*objs)
    obj = objs.size == 1 ? objs.first : objs
    new(*obj_to_vec2_a(obj))
  end

  def self.zero
    new(0.0, 0.0)
  end

end