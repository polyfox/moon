# small data requirement
# eventually this will be implemented in C++
module Moon
  class Rect

    alias :w :width
    alias :w= :width=
    alias :h :height
    alias :h= :height=

    def set(*args)
      self.x, self.y, self.w, self.h = Rect.obj_to_rect_a(args.size > 1 ? args : args.first)
    end

    def to_a
      [x, y, width, height]
    end

    def to_h
      { x: x, y: y, width: width, height: height }
    end

    def x2
      x + width
    end

    def y2
      y + height
    end

    def xy
      Vector2.new x, y
    end

    def xy=(other)
      self.x, self.y = *Vector2.obj_to_vec2_a(other)
    end

    def xyz
      Vector3.new x, y, 0
    end

    def xyz=(other)
      self.x, self.y, _ = *Vector3.obj_to_vec3_a(other)
    end

    def wh
      Vector2.new width, height
    end

    def wh=(other)
      self.w, self.h = *Vector2.obj_to_vec2_a(other)
    end

    def whd
      Vector3.new width, height, 0
    end

    def whd=(other)
      self.w, self.h, _ = *Vector3.obj_to_vec3_a(other)
    end

    ##
    # we don't dup rects nicely...
    def dup
      Moon::Rect.new x, y, width, height
    end

    def self.obj_to_rect_a(obj)
      case obj
      when Moon::Rect
        return *obj
      when Array
        case obj.size
        when 2
          return [*obj[0], *obj[1]]
        # x, y, w, h
        when 4 then
          return *obj
        else
          raise ArgumentError,
                "wrong Array size #{obj.size} (expected 1, 2 or 4)"
        end
      else
        p obj
        raise TypeError,
              "wrong argument type #{obj.class.inspect} (expected Rect or Array)"
      end
    end

    def self.[](*objs)
      obj = objs.size == 1 ? objs.first : objs
      new(*obj_to_rect_a(obj))
    end

  end
end