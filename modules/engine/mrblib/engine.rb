module Moon
  class EngineQuit < Exception
  end

  class Engine
    attr_reader :window
    attr_reader :screen
    attr_reader :input

    def initialize(&block)
      @window = nil
      @screen = nil
      @input = nil
      @fps = Moon::Clock.new
      @step = block
      @log = STDERR
    end

    def uptime
      GLFW.time
    end

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
      GLFW.init

      GLFW.default_window_hints
      GLFW.window_hint GLFW::RESIZABLE, GL2::GL_FALSE
      GLFW.window_hint GLFW::CONTEXT_VERSION_MAJOR, 3
      GLFW.window_hint GLFW::CONTEXT_VERSION_MINOR, 3
      GLFW.window_hint GLFW::OPENGL_FORWARD_COMPAT, GL2::GL_TRUE # for 3.0
      GLFW.window_hint GLFW::OPENGL_PROFILE, GLFW::OPENGL_CORE_PROFILE # for 3.0 and on
      Moon::Shader.is_legacy = false

      begin
        @window = GLFW::Window.new 800, 600, 'Moon Player'
      rescue GLFWError
        GLFW.default_window_hints
        GLFW.window_hint GLFW::CONTEXT_VERSION_MAJOR, 2
        GLFW.window_hint GLFW::CONTEXT_VERSION_MINOR, 1
        Moon::Shader.is_legacy = true

        @window = GLFW::Window.new 800, 600, 'Moon Player'
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

    def setup
      setup_glfw
      setup_window
      setup_opengl
      create_screen
      create_input
      setup_glew
      self
    end

    #
    def shutdown
      @window.destroy
      @window = nil
    end

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
      shutdown
    end
  end
end
