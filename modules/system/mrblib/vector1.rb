module Moon
  class Vector1
    include Comparable

    # @!attribute x
    #  @return [Integer] x coordinate

    # @return [Integer]
    def <=>(other)
      x <=> Vector1.extract(other)[0]
    end

    alias :modulo :%
  end
end
