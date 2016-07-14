module Moon
  # A 2 component vector
  class Vector2
    include Comparable

    # @!attribute x
    #  @return [Integer] x coordinate
    # @!attribute y
    #  @return [Integer] y coordinate

    # @!method initialize_copy(other)
    #   Sets properties from +other+ for +self+
    #   @param [Vector2] other
    #   @return [self]

    # Compares self and other, returns -1 if self is less than other, 0 if they are
    # equal, and 1 if self is greater than
    #
    # @return [Integer]
    def <=>(other)
      [x, y] <=> Vector2.extract(other)
    end

    # @!method +(other)
    #   Returns the sum of self and other
    #   @param [Numeric, Vector2] other
    #   @return [Vector2]

    # @!method -(other)
    #   Returns the difference of self and other
    #   @param [Numeric, Vector2] other
    #   @return [Vector2]

    # @!method *(other)
    #   Returns the cross product of self and other
    #   @param [Numeric, Vector2] other
    #   @return [Vector2]

    # @!method /(other)
    #   Returns the quotient of self and other
    #   @param [Numeric, Vector2] other
    #   @return [Vector2]

    # @!method %(other)
    #   Returns the modulo of self and other
    #   @param [Numeric, Vector2] other
    #   @return [Vector2]

    # @!method <<(other)
    #   Returns the bitwise left-shift result of self and other
    #   @param [Numeric, Vector2] other
    #   @return [Vector2]

    # @!method >>(other)
    #   Returns the bitwise right-shift result of self and other
    #   @param [Numeric, Vector2] other
    #   @return [Vector2]

    # @!method &(other)
    #   Returns the bitwise AND result of self and other
    #   @param [Numeric, Vector2] other
    #   @return [Vector2]

    # @!method |(other)
    #   Returns the bitwise OR result of self and other
    #   @param [Numeric, Vector2] other
    #   @return [Vector2]

    # @!method ^(other)
    #   Returns the bitwise XOR result of self and other
    #   @param [Numeric, Vector2] other
    #   @return [Vector2]

    # @!method set(*args)
    #   Sets all components of self
    #   @overload set(vec)
    #     @param [Vector2, Array<Numeric>[2]] vec
    #   @overload set(x, y)
    #     Sets the value of the vector
    #     @param [Numeric] x
    #     @param [Numeric] y
    #   @return [self]

    # @!method dot(other)
    #   Returns the dot product of self and other, i.e., result = self * other.
    #   @param [Vector2] other
    #   @return [Float]

    # @see #%
    alias :modulo :%

    class << self
      # @!method extract(*args)
      #   Attempts to extract components from the given object to create a vector2
      #   @return [Array<Numeric>[2]] vector2 components

      # @!method [](*args)
      #   Attempts to convert the given object or objects to a Vector2
      #   @return [Vector2]

      # @!method disk_rand(angle)
      #   @return [Vector2]
      # @!method circular_rand(angle)
      #   @return [Vector2]
    end
  end
end
