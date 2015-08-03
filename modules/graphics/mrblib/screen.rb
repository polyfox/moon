module Moon
  class Screen
    attr_reader :w
    attr_reader :h
    attr_reader :rect
    attr_reader :scale
    attr_reader :clear_color
    attr_reader :window
    attr_accessor :log

    # @param [Integer] w
    # @param [Integer] h
    def initialize(w, h)
      @scale = 1.0
      @log = STDERR
      create_window w, h
      initialize_clear_color
      initialize_screen_size
    end

    def create_window(w, h)
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
    end

    # Called by the engine to setup the screen after all the horrible, horrible
    # backend stuff is initialized
    def setup
      OpenGL.reset_flags
    end

    def gl_version
      "#{@window.window_attrib(GLFW::CONTEXT_VERSION_MAJOR).to_s}.#{@window.window_attrib(GLFW::CONTEXT_VERSION_MINOR).to_s}"
    end

    def glsl_version
      OpenGL.glsl_version
    end

    def print_versions
      @log.puts "OpenGL v" + gl_version
      @log.puts "GLSL v" + glsl_version
      @log.puts "GLFW v" + GLFW.version_string
    end

    def make_current
      @window.make_current
    end

    def should_close?
      @window.should_close?
    end

    def swap
      @window.swap_buffers
    end

    def close
      @window.destroy if @window
      @window = nil
    end

    private def initialize_clear_color
      @clear_color = Vector4.new 0, 0, 0, 0
      self.clear_color = @clear_color
    end

    private def initialize_screen_size
      resize(*@window.window_size)
    end

    # @param [Vector4] color
    def clear_color=(color)
      OpenGL.clear_color = Vector4[color].to_a
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

    def title=(title)
      @window.title = title
    end
  end
end
