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
      r, g, b, a = *Color.extract(args.size > 1 ? args : args.first)
      self.r, self.g, self.b, self.a = r.to_f, g.to_f, b.to_f, a.to_f
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

    def rgb
      Color.new r, g, b, 1.0
    end

    def rgb=(other)
      self.r, self.g, self.b, _ = *Color.extract(other)
    end

    def self.extract(obj)
      case obj
      when Moon::Vector3
        return *obj, 1.0
      when Moon::Color
        return *obj
      when Array
        case obj.size
        when 3
          return *obj, 1.0
        # x, y, w, h
        when 4 then
          return *obj
        else
          raise ArgumentError,
                "wrong Array size #{obj.size} (expected 3 or 4)"
        end
      when Numeric
        return obj, obj, obj, 1.0
      else
        raise TypeError,
              "wrong argument type #{obj.class.inspect} (expected Color or Array)"
      end
    end

  end
end