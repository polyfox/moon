module Moon
  class Text

    attr_accessor :color    # Moon::Color
    attr_reader :font     # Moon::Font
    attr_accessor :position # Moon::Vector3
    attr_reader :string   # String
    attr_reader :width
    attr_reader :height

    def font=(new_font)
      @font = new_font
      refresh_size
    end

    def string=(new_string)
      @string = new_string != nil ? new_string.to_s : nil
      refresh_size
    end

    def initialize(string=nil, font=nil)
      @width = 0
      @height = 0
      @string = string
      @font = font
      @position = Moon::Vector3.new(0, 0, 0)
      @color = Moon::Color.new(255, 255, 255, 255)
      refresh_size
    end

    def x
      @position.x
    end

    def y
      @position.y
    end

    def z
      @position.z
    end

    def render(x, y, z)
      if @font && @string
        pos = @position + [x, y, z]
        font.render(pos.x, pos.y + @font.size, pos.z, @string, @color)
      end
    end

    def refresh_size
      if @string
        @width, @height = *@font.calc_bounds(@string)
        @height += @font.size
      else
        @width, @height = 0, 0
      end
    end

  end
end