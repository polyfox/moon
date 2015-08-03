module Moon
  class Screen
    attr_reader :w
    attr_reader :h
    attr_reader :rect
    attr_reader :scale
    attr_reader :clear_color
    attr_accessor :log

    # @param [Integer] w
    # @param [Integer] h
    def initialize(w, h)
      GLFW.window_hint GLFW::RESIZABLE, GL2::GL_FALSE
      GLFW.window_hint GLFW::CONTEXT_VERSION_MAJOR, 3
      GLFW.window_hint GLFW::CONTEXT_VERSION_MINOR, 3
      GLFW.window_hint GLFW::OPENGL_FORWARD_COMPAT, GL2::GL_TRUE # for 3.0
      GLFW.window_hint GLFW::OPENGL_PROFILE, GLFW::OPENGL_CORE_PROFILE # for 3.0 and on
      Moon::Shader.is_legacy = false

      title = 'Moon Player'
      begin
        @window = GLFW::Window.new w, h, title
      rescue GLFWError
        GLFW.default_window_hints
        GLFW.window_hint GLFW::CONTEXT_VERSION_MAJOR, 2
        GLFW.window_hint GLFW::CONTEXT_VERSION_MINOR, 1
        Moon::Shader.is_legacy = true

        @window = GLFW::Window.new w, h, title
      end

      @scale = 1.0
      @log = STDERR
      init_clear_color
      init_shader
    end

    def gl_version
      '' << @window.window_attrib(GLFW::CONTEXT_VERSION_MAJOR) <<
      '.' << @window.window_attrib(GLFW::CONTEXT_VERSION_MINOR)
    end

    def glsl_version
      GL2.glGetString(GL2::GL_SHADING_LANGUAGE_VERSION)
    end

    def print_versions
      @log.puts "OpenGL v" + @screen.gl_version
      @log.puts "GLSL v" + @screen.glsl_version
      @log.puts "GLFW v" + GLFW.version_string
    end

    def make_current
      @window.make_current
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
