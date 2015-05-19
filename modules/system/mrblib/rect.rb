module Moon
  class Rect
    def to_a
      [x, y, w, h]
    end

    def ==(other)
      if other.is_a?(Moon::Rect)
        x == other.x && y == other.y && w == other.w && h == other.h
      else
        false
      end
    end
  end
end
