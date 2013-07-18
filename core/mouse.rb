class Input
  module Mouse
    def self.pos
      [x, y]
    end

    def self.in_area?(x, y, width, height)
      x().between?(x, x+width) && y().between?(y, y+height)
    end
  end
end