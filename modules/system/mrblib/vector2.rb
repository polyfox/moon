module Moon
  class Vector2
    include Comparable

    # @!attribute x
    #  @return [Integer] x coordinate
    # @!attribute y
    #  @return [Integer] y coordinate

    # @return [Integer]
    def <=>(other)
      [x, y] <=> Vector2.extract(other)
    end

    alias :modulo :%
  end
end
