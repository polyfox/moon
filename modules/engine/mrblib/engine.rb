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
    end

    def uptime
      GLFW.time
    end

    def quit
      raise EngineQuit
    end

    private def setup_opengl
      GL2.glEnable GL2::GL_TEXTURE_2D
      GL2.glDisable GL2::GL_DITHER
      #GL2.glDisable GL2::GL_DEPTH_TEST
      GL2.glDisable GL2::GL_CULL_FACE
      GL2.glEnable GL2::GL_BLEND
      GL2.glBlendFunc GL2::GL_SRC_ALPHA, GL2::GL_ONE_MINUS_SRC_ALPHA

      error = GL2.glGetError
      if error != GL2::GL_NO_ERROR
        fail "Error initializing OpenGL! glGetError: #{error}"
      end
    end

    private def setup_glfw
      GLFW.init

      GLFW.default_window_hints
      GLFW.window_hint GLFW::RESIZABLE, GL2::GL_FALSE
      GLFW.window_hint GLFW::CONTEXT_VERSION_MAJOR, 2
      GLFW.window_hint GLFW::CONTEXT_VERSION_MINOR, 1
    end

    private def create_window
      @window = GLFW::Window.new 800, 600, 'Moon Player'
      @window.make_current
      printf "OpenGL v%d.%d\n", @window.window_attrib(GLFW::CONTEXT_VERSION_MAJOR),
                                @window.window_attrib(GLFW::CONTEXT_VERSION_MINOR)
      puts "GLSL v" + GL2.glGetString(GL2::GL_SHADING_LANGUAGE_VERSION)
      puts "GLFW v" + GLFW.version_string
      self
    end

    private def create_screen
      @screen = Screen.new @window
    end

    private def create_input
      @input = Input.new self
    end

    private def setup_glew
      GLEW.init
    end

    def setup
      setup_glfw
      create_window
      setup_opengl
      create_screen
      create_input
      setup_glew
      self
    end

    def shutdown
      @window.destroy
      @window = nil
    end

    def main
      c = GL2::GL_COLOR_BUFFER_BIT | GL2::GL_DEPTH_BUFFER_BIT
      until @window.should_close?
        GL2.glClear c
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
