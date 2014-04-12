#
# moon/core/vector3.rb
#   Everyone loves a Vector class
class Vector3

  include Comparable

  attr_accessor :x
  attr_accessor :y
  attr_accessor :z

  def initialize(x=0.0, y=0.0, z=0.0)
    @x = x
    @y = y
    @z = z
  end

  def <=>(other)
    ox, oy, oz = *Vector3.obj_to_vec3_a(other)
    [ox, oy, oz] <=> [@x, @y, @z]
  end

  def -@
    Vector3.new(-@x, -@y, -@z)
  end

  def +@
    dup
  end

  def coerce(a)
    return self, a
  end

  def to_a
    return @x, @y, @z
  end

  def to_h
    { x: @x, y: @y, z: @z }
  end

  def round(*a)
    @x = @x.round(*a)
    @y = @y.round(*a)
    @z = @z.round(*a)
    self
  end

  def floor
    @x = @x.floor
    @y = @y.floor
    @z = @z.floor
    self
  end

  def ceil
    @x = @x.ceil
    @y = @y.ceil
    @z = @z.ceil
    self
  end

  def normalize
    m = [x, y, z].max.to_f
    Vector3.new x / m, y / m, z / m
  end

  def set(*args)
    @x, @y, @z = *Vector3.obj_to_vec3_a(args.size > 1 ? args : args.first)
    self
  end

  def +(other)
    x, y, z = *Vector3.obj_to_vec3_a(other)
    Vector3.new @x + x, @y + y, @z + z
  end

  def -(other)
    x, y, z = *Vector3.obj_to_vec3_a(other)
    Vector3.new @x - x, @y - y, @z - z
  end

  def *(other)
    x, y, z = *Vector3.obj_to_vec3_a(other)
    Vector3.new @x * x, @y * y, @z * z
  end

  def /(other)
    x, y, z = *Vector3.obj_to_vec3_a(other)
    Vector3.new @x / x, @y / y, @z / z
  end

  def dot(other)
    x, y, z = *Vector3.obj_to_vec3_a(other)
    @x * x + @y * y + @z * z
  end

  #def cross(other)
  #  x, y, z = *obj_to_vec3_a other
  #  @x * x + @y * y + @z * z
  #end

  def xy
    Vector2.new @x, @y
  end

  def xy=(other)
    @x, @y = *Vector2.obj_to_vec2_a(other)
  end

  def xyz
    dup
  end

  def xyz=(other)
    @x, @y, @z = *Vector3.obj_to_vec3_a(other)
  end

  def self.obj_to_vec3_a(obj)
    case obj
    when Vector3 then return *obj
    when Numeric then return obj, obj, obj
    when Array
      if obj.size != 3
        raise ArgumentError,
              "wrong Array size #{obj.size} (expected Array of size 3)"
      end
      return *obj
    else
      raise TypeError,
            "wrong argument type #{obj.class} (expected Array, Numeric or Vector3)"
    end
  end

  def self.[](*objs)
    obj = objs.size == 1 ? objs.first : objs
    new(*obj_to_vec3_a(obj))
  end

  def self.zero
    new(0.0, 0.0, 0.0)
  end

end