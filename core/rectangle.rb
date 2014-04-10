# small data requirement
# eventually this will be implemented in C++
module Moon
  class Rect

    alias :w :width
    alias :w= :width=
    alias :h :height
    alias :h= :height=

    def to_a
      [x, y, width, height]
    end

    def to_h
      { x: x, y: y, width: width, height: height }
    end

    def x2
      x + width
    end

    def y2
      y + height
    end

    def xy
      Vector2.new x, y
    end

    def xyz
      Vector3.new x, y, 0
    end

    def wh
      Vector2.new width, height
    end

    def whd
      Vector3.new width, height, 0
    end

    ##
    # we don't dup rects nicely...
    def dup
      Rect.new x, y, width, height
    end

  end
end