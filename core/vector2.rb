#
# moon/core/vector2.rb
#   Everyone loves a Vector class
class Vector2

  attr_accessor :x, :y

  def initialize(x, y=x)
    @x = x
    @y = y
  end

  def negate!
    @x = -@x
    @y = -@y
    return self
  end

  def -@
    dup.negate!
  end

  def +@
    dup
  end

  def to_a
    [@x, @y]
  end

  def to_h
    { x: @x, y: @y }
  end

  def add!(other)
    x, y = *other
    y ||= x # in the case of a Numeric
    @x += x 
    @y += y 
    return self
  end

  def +(other)
    dup.add!(other)
  end

  def sub!(other)
    add!(-other)
  end

  def -(other)
    dup.sub!(other)
  end

  def self.[](*objs)
    obj = objs.size == 1 ? objs[0] : objs
    case obj 
    when Array   then new(*obj)  # laziness, instead of checking for exactly 2 args
    when Numeric then new(obj, obj)
    when Vector2 then new(obj.x, obj.y)
    else raise(TypeError, "wrong argument type #{obj.class}: expected Array, Numeric or Vector2")
    end
  end

end