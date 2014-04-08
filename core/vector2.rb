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

  def obj_to_vec2a(obj)
    case obj
    when Numeric then return obj, obj
    when Vector2 then return *obj
    else
      raise TypeError,
            "wrong argument type #{obj.class} (expected Numeric or Vector2)"
    end
  end

  def +(other)
    x, y = *obj_to_vec2a(other)
    Vector2.new(@x + x, @y + y)
  end

  def -(other)
    x, y = *obj_to_vec2a(other)
    Vector2.new(@x - x, @y - y)
  end

  def *(other)
    x, y = *obj_to_vec2a(other)
    Vector2.new(@x * x, @y * y)
  end

  def /(other)
    x, y = *obj_to_vec2a(other)
    Vector2.new(@x / x, @y / y)
  end

  def dot(other)
    x, y = *obj_to_vec3a(other)
    @x * x + @y * y
  end

  #def cross(other)
  #  x, y = *obj_to_vec3a other
  #  @x * x + @y * y + @z * z
  #end

  def self.[](*objs)
    obj = objs.size == 1 ? objs[0] : objs
    case obj
    when Array   then new(*obj)  # laziness, instead of checking for exactly 2 args
    when Numeric then new(obj, obj)
    when Vector2 then new(obj.x, obj.y)
    else raise(TypeError, "wrong argument type #{obj.class}: expected Array, Numeric or Vector2")
    end
  end

  private :obj_to_vec2a

end