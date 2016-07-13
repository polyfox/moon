module Moon
  # A 4 component vector, used for primarily Color
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

    # @!method initialize_copy(other)
    #   Sets properties from +other+ for +self+
    #   @param [Vector4] other
    #   @return [self]

    # Compares self and other, returns -1 if self is less than other, 0 if they are
    # equal, and 1 if self is greater than
    #
    # @return [Integer]
    def <=>(other)
      [x, y, z, w] <=> Vector4.extract(other)
    end

    # @!method +(other)
    #   Returns the sum of self and other
    #   @param [Numeric, Vector4] other
    #   @return [Vector4]

    # @!method -(other)
    #   Returns the difference of self and other
    #   @param [Numeric, Vector4] other
    #   @return [Vector4]

    # @!method *(other)
    #   Returns the cross product of self and other
    #   @param [Numeric, Vector4] other
    #   @return [Vector4]

    # @!method /(other)
    #   Returns the quotient of self and other
    #   @param [Numeric, Vector4] other
    #   @return [Vector4]

    # @!method %(other)
    #   Returns the modulo of self and other
    #   @param [Numeric, Vector4] other
    #   @return [Vector4]

    # @!method <<(other)
    #   Returns the bitwise left-shift result of self and other
    #   @param [Numeric, Vector4] other
    #   @return [Vector4]

    # @!method >>(other)
    #   Returns the bitwise right-shift result of self and other
    #   @param [Numeric, Vector4] other
    #   @return [Vector4]

    # @!method &(other)
    #   Returns the bitwise AND result of self and other
    #   @param [Numeric, Vector4] other
    #   @return [Vector4]

    # @!method |(other)
    #   Returns the bitwise OR result of self and other
    #   @param [Numeric, Vector4] other
    #   @return [Vector4]

    # @!method ^(other)
    #   Returns the bitwise XOR result of self and other
    #   @param [Numeric, Vector4] other
    #   @return [Vector4]

    # @!method set(*args)
    #   Sets all components of self
    #   @overload set(vec)
    #     @param [Vector4, Array<Numeric>[4]] vec
    #   @overload set(a, b, c)
    #     @param [Array<Numeric>, Numeric, Vector1, Vector2, Vector3] a
    #     @param [Array<Numeric>, Numeric, Vector1, Vector2, Vector3] b
    #     @param [Array<Numeric>, Numeric, Vector1, Vector2, Vector3] c
    #   @overload set(x, y, z, w)
    #     Sets the value of the vector
    #     @param [Numeric] x
    #     @param [Numeric] y
    #     @param [Numeric] z
    #   @return [self]

    # @!method dot(other)
    #   Returns the dot product of self and other, i.e., result = self * other.
    #   @param [Vector4] other
    #   @return [Float]

    # @see #%
    alias :modulo :%

    class << self
      # @!method extract(*args)
      #   Attempts to extract components from the given objects to create a vector4
      #   @return [Array<Numeric>[4]] vector4 components

      # @!method [](*args)
      #   Attempts to convert the given object or objects to a Vector3
      #   @return [Vector3]
    end
  end
end
