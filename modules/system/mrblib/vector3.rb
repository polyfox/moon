module Moon
  # A 3 component vector
  class Vector3
    include Comparable

    # @!attribute x
    #  @return [Integer] x coordinate
    # @!attribute y
    #  @return [Integer] y coordinate
    # @!attribute z
    #  @return [Integer] z coordinate

    # @!method initialize_copy(other)
    #   Sets properties from +other+ for +self+
    #   @param [Vector3] other
    #   @return [self]

    # Compares self and other, returns -1 if self is less than other, 0 if they are
    # equal, and 1 if self is greater than
    #
    # @return [Integer]
    def <=>(other)
      [x, y, z] <=> Vector3.extract(other)
    end

    # @!method +(other)
    #   Returns the sum of self and other
    #   @param [Numeric, Vector3] other
    #   @return [Vector3]

    # @!method -(other)
    #   Returns the difference of self and other
    #   @param [Numeric, Vector3] other
    #   @return [Vector3]

    # @!method *(other)
    #   Returns the cross product of self and other
    #   @param [Numeric, Vector3] other
    #   @return [Vector3]

    # @!method /(other)
    #   Returns the quotient of self and other
    #   @param [Numeric, Vector3] other
    #   @return [Vector3]

    # @!method %(other)
    #   Returns the modulo of self and other
    #   @param [Numeric, Vector3] other
    #   @return [Vector3]

    # @!method <<(other)
    #   Returns the bitwise left-shift result of self and other
    #   @param [Numeric, Vector3] other
    #   @return [Vector3]

    # @!method >>(other)
    #   Returns the bitwise right-shift result of self and other
    #   @param [Numeric, Vector3] other
    #   @return [Vector3]

    # @!method &(other)
    #   Returns the bitwise AND result of self and other
    #   @param [Numeric, Vector3] other
    #   @return [Vector3]

    # @!method |(other)
    #   Returns the bitwise OR result of self and other
    #   @param [Numeric, Vector3] other
    #   @return [Vector3]

    # @!method ^(other)
    #   Returns the bitwise XOR result of self and other
    #   @param [Numeric, Vector3] other
    #   @return [Vector3]

    # @!method set(*args)
    #   Sets all components of self
    #   @overload set(vec)
    #     @param [Vector3, Array<Numeric>[3]] vec
    #   @overload set(a, b)
    #     @param [Array<Numeric>, Numeric, Vector1, Vector2] a
    #     @param [Array<Numeric>, Numeric, Vector1, Vector2] b
    #   @overload set(x, y, z)
    #     Sets the value of the vector
    #     @param [Numeric] x
    #     @param [Numeric] y
    #     @param [Numeric] z
    #   @return [self]

    # @!method dot(other)
    #   Returns the dot product of self and other, i.e., result = self * other.
    #   @param [Vector3] other
    #   @return [Float]

    # @see #%
    alias :modulo :%

    class << self
      # @!method extract(*args)
      #   Attempts to extract components from the given objects to create a vector3
      #   @return [Array<Numeric>[3]] vector3 components

      # @!method [](*args)
      #   Attempts to convert the given object or objects to a Vector3
      #   @return [Vector3]
    end
  end
end
