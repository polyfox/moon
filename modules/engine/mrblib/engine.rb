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
    attr_reader :window
    attr_reader :screen
    attr_reader :input

    # @yieldparam [Engine] engine
    # @yieldparam [Float] delta
    def initialize(&block)
      @config = { width: 800, height: 600 }
      @window = nil
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

    private def gl_assert
      error = GL2.glGetError
      if error != GL2::GL_NO_ERROR
        fail "Error initializing OpenGL! glGetError: 0x%08x" % error
      end
    end

    private def setup_opengl
      GL2.glDisable GL2::GL_DITHER
      gl_assert
      GL2.glDisable GL2::GL_CULL_FACE
      gl_assert
      GL2.glEnable GL2::GL_BLEND
      gl_assert
      GL2.glBlendFunc GL2::GL_SRC_ALPHA, GL2::GL_ONE_MINUS_SRC_ALPHA
      gl_assert
    end


    private def setup_glfw
      GLFW.default_window_hints
      GLFW.window_hint GLFW::RESIZABLE, GL2::GL_FALSE
      GLFW.window_hint GLFW::CONTEXT_VERSION_MAJOR, 3
      GLFW.window_hint GLFW::CONTEXT_VERSION_MINOR, 3
      GLFW.window_hint GLFW::OPENGL_FORWARD_COMPAT, GL2::GL_TRUE # for 3.0
      GLFW.window_hint GLFW::OPENGL_PROFILE, GLFW::OPENGL_CORE_PROFILE # for 3.0 and on
      Moon::Shader.is_legacy = false

      w, h = @config.fetch(:width), @config.fetch(:height)
      begin
        @window = GLFW::Window.new w, h, 'Moon Player'
      rescue GLFWError
        GLFW.default_window_hints
        GLFW.window_hint GLFW::CONTEXT_VERSION_MAJOR, 2
        GLFW.window_hint GLFW::CONTEXT_VERSION_MINOR, 1
        Moon::Shader.is_legacy = true

        @window = GLFW::Window.new w, h, 'Moon Player'
      end

      @log.puts 'GLFW initialized'
    end

    private def setup_window
      @window.make_current
      printf "OpenGL v%d.%d\n", @window.window_attrib(GLFW::CONTEXT_VERSION_MAJOR),
                                @window.window_attrib(GLFW::CONTEXT_VERSION_MINOR)
      puts "GLSL v" + GL2.glGetString(GL2::GL_SHADING_LANGUAGE_VERSION)
      puts "GLFW v" + GLFW.version_string
      self
    end

    private def create_screen
      @screen = Screen.new @window
      @log.puts 'Screen initialized'
    end

    private def create_input
      @input = Input.new self
      @log.puts 'Input initialized'
    end

    private def setup_glew
      # http://openglbook.com/blog/glgenvertexarrays-access-violationsegfault-with-glew/
      GLEW.experimental = true
      GLEW.init
      @log.puts 'GLEW initialized'
    end

    # @return [self]
    def setup
      setup_glfw
      setup_window
      setup_opengl
      create_screen
      create_input
      setup_glew
      self
    end

    # Destroys the current window and cleans up.
    def shutdown
      @window.destroy if @window
      @window = nil
      self
    end

    # Starts the main loop, terminate the loop using {#quit}
    def main
      @log.puts 'Starting main loop'
      clear_bits = GL2::GL_COLOR_BUFFER_BIT | GL2::GL_DEPTH_BUFFER_BIT
      until @window.should_close?
        GL2.glClear clear_bits
        Audio.update
        @step.call self, @fps.restart
        @window.title = sprintf "FPS: %d", @fps.fps
        @window.swap_buffers
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
