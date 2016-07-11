module Moon
  class Vector3
    include Comparable

    # @!attribute x
    #  @return [Integer] x coordinate
    # @!attribute y
    #  @return [Integer] y coordinate
    # @!attribute z
    #  @return [Integer] z coordinate

    # @return [Integer]
    def <=>(other)
      [x, y, z] <=> Vector3.extract(other)
    end

    alias :modulo :%
  end
end
