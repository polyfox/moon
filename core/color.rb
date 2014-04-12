module Moon
  class Color

    alias :r :red
    alias :r= :red=
    alias :g :green
    alias :g= :green=
    alias :b :blue
    alias :b= :blue=

    def dup
      Color.new red, green, blue, alpha
    end

    def to_a
      return r, g, b, a
    end

    def to_h
      { r: r, g: g, b: b, a: a }
    end

  end
end