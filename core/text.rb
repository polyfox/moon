module Moon
  class Text

    attr_accessor :color    # Moon::Vector4
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

    def initialize(string=nil, font=nil, align=:left)
      @width = 0
      @height = 0
      @string = string
      @font = font
      @position = Moon::Vector3.new(0, 0, 0)
      @color = Moon::Vector4.one
      @align = align
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

    def line_height
      @font.size * 1.2
    end

    def render(x, y, z)
      if @font && @string
        @string.split("\n").each_with_index do |line, index|
          pos = @position + [x, y, z]

          case @align
          when :left
            # do nothing
          when :right
            pos.x -= @font.calc_bounds(line)
          when :center
            pos.x -= @font.calc_bounds(line) / 2
          end

          font.render(pos.x, pos.y + index * line_height, pos.z, line, @color)

        end
      end
    end

    def refresh_size
      if @string
        vec2 = @string.split("\n").inject(Vector2.new(0, 0)) do |vec2, line|
          bounds = @font.calc_bounds(line)
          vec2.x = bounds[0] if vec2.x < bounds[0]
          vec2.y += line_height
          vec2.y += 2 # compensate for outline
          vec2
        end
        @width, @height = *vec2.floor
      else
        @width, @height = 0, 0
      end
    end

  end
end