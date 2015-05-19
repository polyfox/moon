module Moon
  class Vector3
    include Comparable

    # @return [Integer]
    def <=>(other)
      [x, y, z] <=> Vector3.extract(other)
    end

    alias :modulo :%
  end
end
