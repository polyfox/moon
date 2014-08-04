module Moon
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
      self
    end

    def set(*args)
      @x, @y, @z, @width, @height, @depth = *self.class.extract(args.size > 1 ? args : args.first)
      self
    end

    def move(*args)
      @x, @y, @z = *Vector3.extract(args.size > 1 ? args : args.first)
      self
    end

    def resize(*args)
      @width, @height, @depth = *Vector3.extract(args.size > 1 ? args : args.first)
      self
    end

    def xy
      Vector2.new @x, @y
    end

    def xy=(other)
      @x, @y = *Vector2.extract(other)
    end

    def xyz
      Vector3.new @x, @y, @z
    end

    def xyz=(other)
      @x, @y, @z = *Vector3.extract(other)
    end

    def wh
      Vector2.new @width, @height
    end

    def wh=(other)
      @width, @height = *Vector2.extract(other)
    end

    def whd
      Vector3.new @width, @height, @depth
    end

    ###
    #
    ###
    def whd=(other)
      @width, @height, @depth = *Vector3.extract(other)
    end

    ###
    # Converts a given Object to Cuboid array
    # @param [Object] obj
    # @return [Array<Numeric>] (x, y, z, w, h, d)
    ###
    def self.extract(obj)
      case obj
      when Moon::Cuboid
        return *obj
      when Array
        case obj.size
        when 2
          pos, size = *obj
          x, y, z = Vector3.extract(pos)
          w, h, d = Vector3.extract(size)
          return x, y, z, w, h, d
        when 6
          return *obj
        else
          raise ArgumentError, "expected Array of size 2 or 6" if obj.size != 6
        end
      when Numeric
        return 0, 0, 0, obj, obj, obj
      else
        raise TypeError,
              "wrong argument type #{obj.class} (expected Array or Numeric)"
      end
    end

    def self.[](*objs)
      obj = objs.size == 1 ? objs.first : objs
      new(*extract(obj))
    end

    alias :position :xyz
    alias :size :whd
  end
end
