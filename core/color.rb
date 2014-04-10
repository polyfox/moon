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

  end
end