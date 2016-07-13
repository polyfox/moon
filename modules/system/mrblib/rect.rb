module Moon
  # Basic structure for representing a rectangle
  # @example
  #   rect = Moon::Rect.new(4, 16, 32, 32)
  class Rect
    # @!attribute [rw] x
    #  @return [Integer] x coordinate
    # @!attribute [rw] y
    #  @return [Integer] y coordinate
    # @!attribute [rw] w
    #  @return [Integer] rectangle width
    # @!attribute [rw] h
    #  @return [Integer] rectangle height

    # Resets all the properties to 0
    #
    # @return [self]
    def clear
      self.x, self.y, self.w, self.h = 0, 0, 0, 0
      self
    end

    # Returns the Rect's properties in an Array
    #
    # @return [Array<Integer>]
    def to_a
      [x, y, w, h]
    end

    # @return [String]
    def to_s
      "#{x},#{y},#{w},#{h}"
    end

    # Compares the Rect against another object, if the other object is a Rect,
    # it will compare their properties, else the result is always false.
    #
    # @param [Object] other
    # @return [Boolean]
    def ==(other)
      if other.is_a?(Moon::Rect)
        x == other.x && y == other.y && w == other.w && h == other.h
      else
        false
      end
    end
  end
end
