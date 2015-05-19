module Moon
  class Vector2
    include Comparable

    # @return [Integer]
    def <=>(other)
      [x, y] <=> Vector2.extract(other)
    end

    alias :modulo :%
  end
end
