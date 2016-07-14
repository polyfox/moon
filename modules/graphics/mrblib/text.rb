module Moon
  # Text is a renderable representation of a string coupled with a specific
  # font. It has support for text aligning, variable line height, color and
  # outlining.
  #
  # The implementation is optimized in that it will only result in one draw call
  # per text object. This is because we use the Font class to generate a static
  # VBO of the string. Modifying the text attributes will regenerate the VBO.
  class Text
    extend TypedAttributes
    include Shadable

    # Width of the text object (bounding box)
    # @return [Integer]
    attr_reader :w
    # Height of the text object (bounding box)
    # @return [Integer]
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
    # @param [Hash<Symbol, Object>] options  (see #set)
    def initialize(font, string = "", options = {})
      @w = 0
      @h = 0
      set_font font
      set_string string
      @angle = 0.0
      @outline = 0
      @line_height = 1.2
      @align = :left
      @shader = self.class.default_shader
      @origin = Vector2.new(0, 0)
      @color = Vector4.new(1, 1, 1, 1)
      @outline_color = Vector4.new(0, 0, 0, 1)
      @vbo = VertexBuffer.new(VertexBuffer::DYNAMIC_DRAW)
      @transform = Matrix4.new
      @rotation_matrix = Matrix4.new
      @mode = OpenGL::TRIANGLES
      set(options)
    end

    # Sets multiple properties at once, this will regenerate the buffers just
    # once after setting all the values.
    #
    # @param [Hash<Symbol, Object>] options
    # @option options [Font] :font
    # @option options [String] :string
    # @option options [Float] :angle
    # @option options [Integer] :outline
    # @option options [Float] :line_height
    # @option options [Symbol] :align  either :left, :right, or :center
    # @option options [Shader] :shader
    # @option options [Shader] :origin
    # @option options [Vector4] :color
    # @option options [Vector4] :outline_color
    # @return [self]
    def set(options)
      unless options.empty?
        set_font options.fetch(:font, @font)
        set_string options.fetch(:string, @string)
        self.angle = options.fetch(:angle, @angle)
        set_outline options.fetch(:outline, @outline)
        set_line_height options.fetch(:line_height, @line_height)
        set_align options.fetch(:align, @align)
        self.shader = options.fetch(:shader, @shader)
        self.origin = options.fetch(:origin, @origin)
        set_color options.fetch(:color, @color)
        set_outline_color options.fetch(:outline_color, @outline_color)
      end
      generate_buffers
      self
    end

    private def generate_buffers
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
    private :set_string
    # Sets a new string for the text
    #
    # @param [String] string
    def string=(string)
      set_string(string)
      generate_buffers
    end

    alias :set_font :font=
    private :set_font
    # Sets a new font for the text
    #
    # @param [Font] font
    def font=(font)
      set_font(font)
      generate_buffers
    end

    alias :set_color :color=
    private :set_color
    # Sets a new color for the text
    #
    # @param [Vector4] color
    def color=(color)
      set_color(color)
      generate_buffers
    end

    alias :set_outline_color :outline_color=
    private :set_outline_color
    # Sets a new outline color for the text
    #
    # @param [Vector4] outline_color
    def outline_color=(outline_color)
      set_outline_color(outline_color)
      generate_buffers
    end

    alias :set_outline :outline=
    private :set_outline
    # Sets a new outline size for the text
    #
    # @param [Integer] outline
    def outline=(outline)
      set_outline(outline)
      generate_buffers
    end

    alias :set_line_height :line_height=
    private :set_line_height
    # Sets a new line_height size for the text
    #
    # @param [Float] line_height
    def line_height=(line_height)
      set_line_height(line_height)
      generate_buffers
    end

    alias :set_align :align=
    private :set_align
    # Sets a new align size for the text
    #
    # @param [Float] align
    def align=(align)
      set_align(align)
      generate_buffers
    end

    # Renders the text on screen at the specified coordinates.
    #
    # @param [Integer] x
    # @param [Integer] y
    # @param [Integer] z
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
