module Moon
  class Vector4
    include Comparable

    # @return [Integer]
    def <=>(other)
      [x, y, z, w] <=> Vector4.extract(other)
    end

    alias :modulo :%
  end
end
