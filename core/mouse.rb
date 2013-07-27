class Input
  module Mouse

    def self.pos
      [x, y]
    end

    def self.in_area?(x, y, width, height)
      self.x.between?(x, x+width) && self.y.between?(y, y+height)
    end
 
    def self.in_rect?(rect)
      in_area?(rect.x, rect.y, rect.width, rect.height)
    end

    def self.triggered?(key_id)
      return pressed?(key_id) == 0
    end

  end
end