module Moon
  class Text
    extend TypedAttributes
    include Shadable

    attr_reader :w
    attr_reader :h

    attribute :align,         Symbol
    attribute :angle,         Float
    attribute :color,         Vector4
    attribute :font,          Font
    attribute :line_height,   Float
    attribute :origin,        Vector2
    attribute :outline,       Integer
    attribute :outline_color, Vector4
    attribute :shader,        Shader
    attribute :string,        String

    # @param [Font] font
    # @param [String] string
    # @param [Hash<Symbol, Object>] options
    # @option options [Integer] :outline
    # @option options [Float] :line_height
    # @option options [Symbol] :align  either :left, :right, or :center
    # @option options [Shader] :shader
    # @option options [Vector4] :color
    # @option options [Vector4] :outline_color
    def initialize(font, string = "", options = {})
      @w = 0
      @h = 0
      set_font font
      set_string string
      self.angle = options.fetch(:angle, 0.0)
      set_outline options.fetch(:outline, 0)
      set_line_height options.fetch(:line_height, 1.2)
      set_align options.fetch(:align, :left)
      self.shader = options.fetch(:shader) { self.class.default_shader }
      self.origin = options.fetch(:origin) { Vector2.new(0, 0) }
      set_color options.fetch(:color) { Vector4.new(1, 1, 1, 1) }
      set_outline_color options.fetch(:outline_color) { Vector4.new(0, 0, 0, 1) }
      @vbo = VertexBuffer.new(VertexBuffer::DYNAMIC_DRAW)
      @transform = Matrix4.new
      @rotation_matrix = Matrix4.new
      @mode = OpenGL::TRIANGLES
      generate_buffers
    end

    def generate_buffers
      @vbo.clear
      lines = @string.split("\n")
      lines.each_with_index do |line, index|
        x = case @align
        when :left
          0 # do nothing
        when :right
          -@font.calc_bounds(line)[0]
        when :center
          -@font.calc_bounds(line)[0] / 2
        end.to_i
        y = (index * @font.size * @line_height).to_i
        add_text(line, x, y)
      end

      @w, @h = @font.calc_bounds(@string, @line_height)
    end

    alias :set_string :string=
    # Sets a new string for the text
    #
    # @param [String] string
    def string=(string)
      set_string(string)
      generate_buffers
    end

    alias :set_font :font=
    # Sets a new font for the text
    #
    # @param [Font] font
    def font=(font)
      set_font(font)
      generate_buffers
    end

    alias :set_color :color=
    # Sets a new color for the text
    #
    # @param [Vector4] color
    def color=(color)
      set_color(color)
      generate_buffers
    end

    alias :set_outline_color :outline_color=
    # Sets a new outline color for the text
    #
    # @param [Vector4] outline_color
    def outline_color=(outline_color)
      set_outline_color(outline_color)
      generate_buffers
    end

    alias :set_outline :outline=
    # Sets a new outline size for the text
    #
    # @param [Integer] outline
    def outline=(outline)
      set_outline(outline)
      generate_buffers
    end

    alias :set_line_height :line_height=
    # Sets a new line_height size for the text
    #
    # @param [Float] line_height
    def line_height=(line_height)
      set_line_height(line_height)
      generate_buffers
    end

    alias :set_align :align=
    # Sets a new align size for the text
    #
    # @param [Float] align
    def align=(align)
      set_align(align)
      generate_buffers
    end

    def render(x, y, z)
      @rotation_matrix.clear
      @rotation_matrix.rotate!(@angle, [0, 0, 1])
      @rotation_matrix.translate!(-@origin.x, -@origin.y, 0)
      @transform.clear
      @transform.translate!(x, y + @font.ascender, z)
      transform = @transform * @rotation_matrix

      @shader.use
      Renderer.instance.render(@shader, @vbo, @font, transform, @mode)
    end
  end
end
