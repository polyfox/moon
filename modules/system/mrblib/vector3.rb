module Moon
  class Vector3
    include Comparable

    # @!method ^(other)
    #   Add two vectors together.
    #   @param [Vector3] other
    #   @return [Vector3]

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
