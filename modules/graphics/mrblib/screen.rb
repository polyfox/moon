module Moon
  class Screen
    attr_reader :w
    attr_reader :h
    attr_reader :rect
    attr_reader :scale
    attr_reader :window
    attr_reader :clear_color

    # @param [GLFW::Window] window
    def initialize(window)
      @window = window
      @scale = 1.0
      init_clear_color
      init_shader
    end

    private def init_clear_color
      @clear_color = Vector4.new 0, 0, 0, 0
      self.clear_color = @clear_color
    end

    private def init_shader
      resize(*@window.window_size)
    end

    def refresh_size
      @w, @h = *@window.window_size
      @rect = Rect.new(0, 0, @w, @h)
    end

    # @param [Vector4] color
    def clear_color=(color)
      @clear_color = Vector4[color]
      GL2.glClearColor @clear_color.r, @clear_color.g, @clear_color.b, @clear_color.a
    end

    def update_projection
      Shader.projection_matrix = Matrix4.ortho 0.0, w / scale, h / scale, 0.0, -1.0, 1.0
    end

    # @param [Integer] w
    # @param [Integer] h
    def resize(w, h)
      @window.window_size = [w, h]
      refresh_size
      update_projection
    end

    # @param [Float] s
    def scale=(s)
      @scale = s
      update_projection
    end
  end
end
