#
# moon/core/vector2.rb
#   Everyone loves a Vector class
class Vector2

  attr_accessor :x
  attr_accessor :y

  def initialize(x=0, y=0)
    @x = x
    @y = y
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

  def obj_to_vec2_a(obj)
    case obj
    when Array
      raise ArgumentError, "expected Array of size 2" if obj.size != 2
      return *obj
    when Numeric then return obj, obj
    when Vector2 then return *obj
    else
      raise TypeError,
            "wrong argument type #{obj.class} (expected Array, Numeric or Vector2)"
    end
  end

  def normalize
    m = [x, y].max.to_f
    Vector2.new x / m, y / m
  end

  def set(*args)
    @x, @y = *obj_to_vec2_a(args.size > 1 ? args : args.first)
    self
  end

  def +(other)
    x, y = *obj_to_vec2_a(other)
    Vector2.new @x + x, @y + y
  end

  def -(other)
    x, y = *obj_to_vec2_a(other)
    Vector2.new @x - x, @y - y
  end

  def *(other)
    x, y = *obj_to_vec2_a(other)
    Vector2.new @x * x, @y * y
  end

  def /(other)
    x, y = *obj_to_vec2_a(other)
    Vector2.new @x / x, @y / y
  end

  def dot(other)
    x, y = *obj_to_vec3a(other)
    @x * x + @y * y
  end

  #def cross(other)
  #  x, y = *obj_to_vec3a other
  #  @x * x + @y * y + @z * z
  #end

  def xy
    dup
  end

  def xyz
    Vector3.new @x, @y, 0
  end

  def self.[](*objs)
    obj = objs.size == 1 ? objs[0] : objs
    case obj
    when Array
      raise ArgumentError, "expected Array of size 3" if obj.size != 2
      new(*obj)
    when Numeric then new obj, obj
    when Vector2 then new obj.x, obj.y
    else
      raise TypeError,
            "wrong argument type #{obj.class} (expected Array, Numeric or Vector2)"
    end
  end

  def self.zero
    new(0.0, 0.0)
  end

  private :obj_to_vec2_a

end