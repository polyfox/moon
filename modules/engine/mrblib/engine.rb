module Moon
  # Error raised by {Engine#quit}, used to terminate a {Engine#main} loop
  class EngineQuit < Exception
  end

  class Engine
    # @!attribute step
    #   @return [Proc] per frame step function
    attr_accessor :step

    attr_accessor :config

    attr_accessor :log
    attr_reader :screen
    attr_reader :input

    # @yieldparam [Engine] engine
    # @yieldparam [Float] delta
    def initialize(&block)
      @config = { width: 800, height: 600 }
      @screen = nil
      @input = nil
      @fps = Moon::Clock.new
      @step = block
      @log = STDERR
    end

    # @return [Float]
    def uptime
      GLFW.time
    end

    # Terminates a running {Engine#main}
    #
    # @raise {EngineQuit}
    def quit
      raise EngineQuit
    end

    private def setup_glfw
      @screen = Screen.new(@config.fetch(:width), @config.fetch(:height))
      @screen.make_current
      # debugging
      @screen.print_versions
      self
    end

    private def create_input
      @input = Input.new @screen.window
      @log.puts 'Input initialized'
    end

    private def setup_glew
      # http://openglbook.com/blog/glgenvertexarrays-access-violationsegfault-with-glew/
      GLEW.experimental = true
      GLEW.init
      @log.puts 'GLEW initialized'
    end

    def setup_default_shaders
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
    end

    # @return [self]
    def setup
      setup_glfw
      setup_glew
      @screen.setup
      create_input
      setup_default_shaders
      self
    end

    # Destroys the current screen and cleans up.
    def shutdown
      @screen.close if @screen
      self
    end

    # Starts the main loop, terminate the loop using {#quit}
    def main
      @log.puts 'Starting main loop'
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
    end

    def run
      setup
      main
    ensure
      shutdown
    end
  end
end
