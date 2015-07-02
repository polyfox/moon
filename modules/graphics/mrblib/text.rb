module Moon
  class Text
    extend TypedAttributes
    include Shadable

    attr_reader :w
    attr_reader :h

    attribute :shader,        Shader
    attribute :string,        String
    attribute :font,          Font
    attribute :color,         Vector4
    attribute :outline,       Integer
    attribute :outline_color, Vector4

    # @param [Font] font
    # @param [String] string
    def initialize(font, string = "")
      set_font font
      set_string string
      @color = Vector4.new(1, 1, 1, 1)
      @outline = 0
      @outline_color = Vector4.new(0, 0, 0, 1)
      @vbo = VertexBuffer.new(VertexBuffer::DYNAMIC_DRAW)
      @shader = self.class.default_shader
      @w = 0
      @h = 0
      generate_buffers
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
    # @param [Vector4] outline
    def outline=(outline)
      set_outline(outline)
      generate_buffers
    end
  end
end
