module Moon
  # Screen represents the actual game window.
  class Screen
    # Screen width
    attr_reader :w
    # Screen height
    attr_reader :h
    attr_reader :rect
    # Scale of the screen coordinates.
    # By default, we map 1:1, each unit is 1px.
    attr_reader :scale
    # The color with which the screen is cleared at the start of every frame.
    # Black by default.
    attr_reader :clear_color
    # Underlying window implementation (in default case GLFW).
    attr_reader :window
    # @return [Moon::ContextLogger, ILogger] any logger like interface
    attr_accessor :logger

    # Creates a new game window with the given width and height.
    #
    # @param [Integer] w
    # @param [Integer] h
    def initialize(w, h)
      @scale = 1.0
      @logger = Moon::ContextLogger.new(STDERR, 'Screen')
      create_window w, h
      initialize_renderer
      initialize_clear_color
      initialize_screen_size
      @vsync = true
    end

    # Creates the internal window
    #
    # @param [Integer] w  width of the window
    # @param [Integer] h  height of the window
    def create_window(w, h)
      @logger.debug "Creating Window: w=#{w} h=#{h}"
      GLFW.window_hint GLFW::RESIZABLE, GL2::GL_FALSE
      GLFW.window_hint GLFW::CONTEXT_VERSION_MAJOR, 3
      GLFW.window_hint GLFW::CONTEXT_VERSION_MINOR, 3
      GLFW.window_hint GLFW::OPENGL_FORWARD_COMPAT, GL2::GL_TRUE # for 3.0
      GLFW.window_hint GLFW::OPENGL_PROFILE, GLFW::OPENGL_CORE_PROFILE # for 3.0 and on
      Moon::Shader.is_legacy = false

      title = 'Moon Player'
      begin
        @window = GLFW::Window.new w, h, title
        @logger.warn "3.3 Window Created"
      rescue GLFWError
        @logger.warn "Failed to obtain 3.3 context, falling back to 2.1"
        GLFW.default_window_hints
        GLFW.window_hint GLFW::CONTEXT_VERSION_MAJOR, 2
        GLFW.window_hint GLFW::CONTEXT_VERSION_MINOR, 1
        Moon::Shader.is_legacy = true

        @window = GLFW::Window.new w, h, title
        @logger.warn "2.1 Window Created"
      end
    end

    # Called by the engine to setup the screen after all the horrible, horrible
    # backend stuff is initialized
    def setup
      OpenGL.reset_flags
    end

    # Returns the OpenGL context version
    #
    # @return [String]
    def gl_version
      "#{@window.window_attrib(GLFW::CONTEXT_VERSION_MAJOR).to_s}.#{@window.window_attrib(GLFW::CONTEXT_VERSION_MINOR).to_s}"
    end

    # Returns the guesstimated version of GLSL supported
    #
    # @return [String]
    def glsl_version
      OpenGL.glsl_version
    end

    # Prints all the available version strings to the @log
    def print_versions
      @logger.info "OpenGL v" + gl_version
      @logger.info "GLSL v" + glsl_version
      @logger.info "GLFW v" + GLFW.version_string
    end

    # Make the screen the current context
    def make_current
      @window.make_current
      Screen.current = self
      self.vsync = @vsync
    end

    # Should the screen close?
    def should_close?
      @window.should_close?
    end

    # Swaps buffers, call this once per frame
    def swap
      @window.swap_buffers
    end

    # Closes the underlying window and cleans up
    def shutdown
      @logger.debug "Shutting Down"
      if @window
        @logger.debug "Closing Window"
        @window.should_close = true
        GLFW.wait_events
        @window.destroy
      end
      @window = nil
      @logger.debug "Shutdown"
      self
    end

    private def initialize_clear_color
      @clear_color = Vector4.new 0, 0, 0, 0
      self.clear_color = @clear_color
    end

    private def initialize_screen_size
      resize(*@window.window_size)
    end

    private def initialize_renderer
      Moon::Renderer.instance = Moon::Renderer.new
    end

    # @param [Vector4] color
    def clear_color=(color)
      @clear_color = Vector4[color]
      OpenGL.clear_color = @clear_color.to_a
    end

    # Updates the Renderer's projection_matrix to match the screen resolution.
    # This is called any time the window is resized.
    def update_projection
      Renderer.instance.projection_matrix = Matrix4.ortho 0.0, w / scale, h / scale, 0.0, -1.0, 1.0
    end

    # Resize the game window to a certain size.
    #
    # @param [Integer] w
    # @param [Integer] h
    def resize(w, h)
      @logger.debug "Resizing Screen to w=#{w} h=#{h}"
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

    attr_reader :title
    # Sets the window title.
    # @param [String] title
    def title=(title)
      @title = title
      @window.title = @title
    end

    attr_reader :vsync
    # Sets the vsync state on / off
    # @param [Boolean] bool
    def vsync=(bool)
      @vsync = bool
      GLFW.swap_interval = @vsync ? 1 : 0
    end

    class << self
      # Represents the currently active Screen.
      # (we can technically create more than one window with GLFW and toggle
      # between them)
      attr_accessor :current
    end
  end
end
