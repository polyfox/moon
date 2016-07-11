module Moon
  class Vector4
    include Comparable

    # @!attribute x
    #  @return [Integer] x coordinate
    # @!attribute y
    #  @return [Integer] y coordinate
    # @!attribute z
    #  @return [Integer] z coordinate
    # @!attribute w
    #  @return [Integer] w coordinate

    # @return [Integer]
    def <=>(other)
      [x, y, z, w] <=> Vector4.extract(other)
    end

    alias :modulo :%
  end
end
