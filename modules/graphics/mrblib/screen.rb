module Moon
  class Screen
    attr_reader :w
    attr_reader :h
    attr_reader :rect
    attr_reader :scale
    attr_reader :window
    attr_reader :clear_color

    # @param [Integer] w
    # @param [Integer] h
    def initialize(w, h)
      GLFW.window_hint GLFW::RESIZABLE, GL2::GL_FALSE
      GLFW.window_hint GLFW::CONTEXT_VERSION_MAJOR, 3
      GLFW.window_hint GLFW::CONTEXT_VERSION_MINOR, 3
      GLFW.window_hint GLFW::OPENGL_FORWARD_COMPAT, GL2::GL_TRUE # for 3.0
      GLFW.window_hint GLFW::OPENGL_PROFILE, GLFW::OPENGL_CORE_PROFILE # for 3.0 and on
      Moon::Shader.is_legacy = false

      begin
        @window = GLFW::Window.new w, h, 'Moon Player'
      rescue GLFWError
        GLFW.default_window_hints
        GLFW.window_hint GLFW::CONTEXT_VERSION_MAJOR, 2
        GLFW.window_hint GLFW::CONTEXT_VERSION_MINOR, 1
        Moon::Shader.is_legacy = true

        @window = GLFW::Window.new w, h, 'Moon Player'
      end

      @scale = 1.0
      init_clear_color
      init_shader
    end

    def close
      @window.destroy if @window
      @window = nil
    end

    private def init_clear_color
      @clear_color = Vector4.new 0, 0, 0, 0
      self.clear_color = @clear_color
    end

    private def init_shader
      resize(*@window.window_size)
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
      @w, @h = *@window.window_size
      @rect = Rect.new(0, 0, @w, @h)
      update_projection
    end

    # @param [Float] s
    def scale=(s)
      @scale = s
      update_projection
    end
  end
end
