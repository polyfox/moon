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

  def set(*args)
    @x, @y = *Vector2.obj_to_vec2_a(args.size > 1 ? args : args.first)
    self
  end

  def round(*a)
    Vector2.new @x.round(*a), @y.round(*a)
  end

  def floor
    Vector2.new @x.floor, @y.floor
  end

  def ceil
    Vector2.new @x.ceil, @y.ceil
  end

  def normalize
    m = [@x, @y].max.to_f
    Vector2.new @x / m, @y / m
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

  def %(other)
    x, y = *Vector2.obj_to_vec2_a(other)
    Vector2.new @x % x, @y % y
  end

  def dot(other)
    x, y = *Vector2.obj_to_vec2_a(other)
    @x * x + @y * y
  end

  def cross(other)
    x, y = *Vector2.obj_to_vec2_a(other)
    Vector2.new @x * y, @y * x
  end

  def rotate(r)
    Vector2.new @x * Math.cos(r) - @y * Math.sin(r),
                @x * Math.sin(r) + @y * Math.cos(r)
  end

  def mag
    Math.sqrt @x * @x + @y * @y
  end

  def rad
    Math.atan2(@y, @x)
  end

  def rad=(n)
    m = mag
    @x = m * Math.cos(n)
    @y = m * Math.sin(n)
  end

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