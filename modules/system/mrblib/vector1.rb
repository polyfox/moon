module Moon
  # A single component vector, operates as a Option class for Floats
  class Vector1
    include Comparable

    # @!attribute x
    #  @return [Integer] x coordinate

    # @!method initialize_copy(other)
    #   Sets properties from +other+ for +self+
    #   @param [Vector1] other
    #   @return [self]

    # Compares self and other, returns -1 if self is less than other, 0 if they are
    # equal, and 1 if self is greater than
    #
    # @return [Integer]
    def <=>(other)
      x <=> Vector1.extract(other)[0]
    end

    # @!method +(other)
    #   Returns the sum of self and other
    #   @param [Numeric, Vector1] other
    #   @return [Vector1]

    # @!method -(other)
    #   Returns the difference of self and other
    #   @param [Numeric, Vector1] other
    #   @return [Vector1]

    # @!method *(other)
    #   Returns the cross product of self and other
    #   @param [Numeric, Vector1] other
    #   @return [Vector1]

    # @!method /(other)
    #   Returns the quotient of self and other
    #   @param [Numeric, Vector1] other
    #   @return [Vector1]

    # @!method %(other)
    #   Returns the modulo of self and other
    #   @param [Numeric, Vector1] other
    #   @return [Vector1]

    # @!method <<(other)
    #   Returns the bitwise left-shift result of self and other
    #   @param [Numeric, Vector1] other
    #   @return [Vector1]

    # @!method >>(other)
    #   Returns the bitwise right-shift result of self and other
    #   @param [Numeric, Vector1] other
    #   @return [Vector1]

    # @!method &(other)
    #   Returns the bitwise AND result of self and other
    #   @param [Numeric, Vector1] other
    #   @return [Vector1]

    # @!method |(other)
    #   Returns the bitwise OR result of self and other
    #   @param [Numeric, Vector1] other
    #   @return [Vector1]

    # @!method ^(other)
    #   Returns the bitwise XOR result of self and other
    #   @param [Numeric, Vector1] other
    #   @return [Vector1]

    # @!method set(value)
    #   Sets the value of the vector
    #   @param [Numeric] value
    #   @return [self]

    # @!method dot(other)
    #   Returns the dot product of self and other
    #   @param [Numeric, Vector1] other
    #   @return [Float]

    # @see #%
    alias :modulo :%

    class << self
      # @!method extract(*args)
      #   Attempts to extract components from the given object to create a vector1
      #   @return [Array<Numeric>[1]] vector1 components

      # @!method [](*args)
      #   Attempts to convert the given object or objects to a Vector1
      #   @return [Vector1]
    end
  end
end
