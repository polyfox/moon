module Moon
  class Color

    alias :r  :red
    alias :r= :red=
    alias :g  :green
    alias :g= :green=
    alias :b  :blue
    alias :b= :blue=
    alias :a  :alpha
    alias :a= :alpha=

    def set(*args)
      r, g, b, a = *Color.obj_to_color_a(args.size > 1 ? args : args.first)
      self.r, self.g, self.b, self.a = r.to_i, g.to_i, b.to_i, a.to_i
    end

    def dup
      Color.new r, g, b, a
    end

    def to_a
      return r, g, b, a
    end

    def to_h
      { r: r, g: g, b: b, a: a }
    end

    def self.obj_to_color_a(obj)
      case obj
      when Moon::Color
        return *obj
      when Array
        case obj.size
        when 3
          return *obj, 255
        # x, y, w, h
        when 4 then
          return *obj
        else
          raise ArgumentError,
                "wrong Array size #{obj.size} (expected 3 or 4)"
        end
      else
        raise TypeError,
              "wrong argument type #{obj.class.inspect} (expected Color or Array)"
      end
    end

  end
end