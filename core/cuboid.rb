class Cuboid

  attr_accessor :x
  attr_accessor :y
  attr_accessor :z
  attr_accessor :width
  attr_accessor :height
  attr_accessor :depth

  alias :w :width
  alias :w= :width=
  alias :h :height
  alias :h= :height=
  alias :d :depth
  alias :d= :depth=

  def initialize(*args)
    clear
    set(*args)
  end

  def to_a
    return @x, @y, @z, @width, @height, @depth
  end

  def to_h
    { x: @x, y: @y, z: @z, width: @width, height: @height, depth: @depth }
  end

  def clear
    @x, @y, @z, @width, @height, @depth = 0, 0, 0, 0, 0, 0
  end

  def obj_to_vec3a(obj)
    case obj
    when Array
      raise ArgumentError, "expected Array of size 3" if obj.size != 3
      return *obj
    when Numeric then return obj, obj, obj
    when Vector3 then return *obj
    else
      raise TypeError,
            "wrong argument type #{obj.class} (expected Array, Numeric or Vector3)"
    end
  end

  def obj_to_cuboid_a(obj)
    case obj
    when Array
      case obj.size
      when 2
        pos, size = *obj
        x, y, z = *pos
        w, h, d = *size
        return x, y, z, w, h, d
      when 6
        return *obj
      else
        raise ArgumentError, "expected Array of size 2 or 6" if obj.size != 6
      end
    when Numeric then return 0, 0, 0, obj, obj, obj
    else
      raise TypeError,
            "wrong argument type #{obj.class} (expected Array or Numeric)"
    end
  end

  def set(*args)
    @x, @y, @z, @width, @height, @depth = *obj_to_cuboid_a(args.size > 1 ? args : args.first)
    self
  end

  def move(*args)
    @x, @y, @z = *obj_to_vec3a(args.size > 1 ? args : args.first)
  end

  def resize(*args)
    @width, @height, @depth = *obj_to_vec3_a(args.size > 1 ? args : args.first)
  end

  def xy
    Vector2.new @x, @y
  end

  def xyz
    Vector3.new @x, @y, @z
  end

  def wh
    Vector2.new @width, @height
  end

  def whd
    Vector3.new @width, @height, @depth
  end

  private :obj_to_cuboid_a
  private :obj_to_vec3_a

end