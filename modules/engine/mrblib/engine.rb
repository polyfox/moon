module Moon
  # Error raised by {Engine#quit}, used to terminate a {Engine#main} loop
  class EngineQuit < Exception
  end

  # Engine wraps all the components with a bow. It's responsible for
  # bootstrapping a new Screen window, setting up OpenGL and Audio.
  class Engine
    # @!attribute step
    #   @return [Proc] per frame step function
    attr_accessor :step
    # @return [Hash<Symbol, Object>]
    attr_accessor :config
    # @return [Moon::ContextLogger, ILogger] any logger like interface
    attr_accessor :logger
    # @return [Moon::Screen]
    attr_reader :screen
    # @return [Moon::Input]
    attr_reader :input

    # Initializes the engine. The block should be a step function to be called
    # on every frame.
    #
    # @yieldparam [Engine] engine
    # @yieldparam [Float] delta time since last frame
    def initialize(&block)
      @config = { width: 800, height: 600 }
      @screen = nil
      @input = nil
      @fps = Moon::Clock.new
      @step = block
      @logger = Moon::ContextLogger.new(STDERR, 'Engine')
    end

    # @return [Float] Returns the time since the engine has started.
    def uptime
      GLFW.time
    end

    # Terminates a running {Engine#main}
    #
    # @raise {EngineQuit}
    def quit
      @logger.debug 'Quitting Engine!'
      raise EngineQuit
    end

    private def setup_glfw
      @logger.debug 'Initializing Screen'
      @screen = Screen.new(@config.fetch(:width), @config.fetch(:height))
      @screen.make_current
      # debugging
      @screen.print_versions
      self
    end

    private def create_input
      @logger.debug 'Initializing Input'
      @input = Input.new @screen.window
      @logger.debug 'Input initialized'
    end

    private def setup_glew
      @logger.debug 'Initializing GLEW'
      # http://openglbook.com/blog/glgenvertexarrays-access-violationsegfault-with-glew/
      GLEW.experimental = true
      GLEW.init
      @logger.debug 'GLEW initialized'
    end

    private def setup_default_shaders
      @logger.debug 'Initializing default shaders'
      @shaders = {}
      sd = Moon::Shader::DEFAULTS
      shader_path = Moon::Shader.is_legacy ? '120' : '330'
      @shaders[:quad] = Moon::Shader.new(
        sd.fetch(File.join(shader_path, 'quad.vert')),
        sd.fetch(File.join(shader_path, 'quad.frag'))
      )
      @shaders[:text] = Moon::Shader.new(
        sd.fetch(File.join(shader_path, 'quad.vert')),
        sd.fetch(File.join(shader_path, 'text.frag'))
      )

      Moon::Sprite.default_shader = @shaders[:quad]
      Moon::Spritesheet.default_shader = @shaders[:quad]
      Moon::Tilemap.default_shader = @shaders[:quad]
      Moon::Text.default_shader = @shaders[:text]
      @logger.debug 'Default Shaders Initialized'
      self
    end

    # Initializes all internal systems and prepares for main loop
    #
    # @return [self]
    def setup
      @logger.debug "Initializing"
      setup_glfw
      setup_glew
      @screen.setup
      create_input
      setup_default_shaders
      @logger.debug "Initialized"
      self
    end

    # Destroys the systems and cleans up
    def shutdown
      @logger.debug "Shutting down"
      @input.shutdown if @input
      @screen.shutdown if @screen
      @logger.debug "Shutdown"
      self
    end

    # Starts the main loop, terminate the loop using {#quit}
    def main
      @logger.info "Audio Module: #{Audio::NAME}"
      @logger.debug 'Starting main loop'
      clear_bits = GL2::GL_COLOR_BUFFER_BIT | GL2::GL_DEPTH_BUFFER_BIT
      until @screen.should_close?
        GL2.glClear clear_bits
        Audio.update
        @step.call self, @fps.restart
        @screen.title = sprintf "FPS: %d", @fps.fps
        @screen.swap
        GLFW.poll_events
      end
    rescue EngineQuit
      @logger.debug 'Got an EngineQuit request'
    end

    # Sets everything up, runs the main loop, and ensures we take care of
    # cleanup on quit.
    def run
      setup
      main
    ensure
      shutdown
    end
  end
end
