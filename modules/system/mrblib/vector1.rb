module Moon
  class Vector1
    include Comparable

    # @return [Integer]
    def <=>(other)
      x <=> Vector1.extract(other)[0]
    end

    alias :modulo :%
  end
end
