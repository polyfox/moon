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

    private def gl_assert
      error = GL2.glGetError
      if error != GL2::GL_NO_ERROR
        if error == 0x500
          # just ignore, invalid enums for now
          return
        end
        begin
          fail "Error initializing OpenGL! glGetError: 0x%08x" % error
        # mruby generates backtraces based on the current backtrace, and
        # it avoids copying the backtrace into the exception, so an exception's
        # backtrace will be based on the CURRENT stack trace rather than
        # the stack trace it was raised from. -.- pain in the a__ I know.
        rescue => ex
          STDERR.puts ex.inspect
          ex.backtrace.each do |line|
            STDERR.puts "\t#{line}"
          end
          raise ex
        end
      end
    end

    # Resets Moon's required GL flags, since some external gems may change
    # them for their own evil ;3
    def reset_gl_flags
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
      w, h = @config.fetch(:width), @config.fetch(:height)
      @screen = Screen.new(w, h)
      @screen.window.make_current

      printf "OpenGL v%d.%d\n", @screen.window.window_attrib(GLFW::CONTEXT_VERSION_MAJOR),
                                @screen.window.window_attrib(GLFW::CONTEXT_VERSION_MINOR)
      puts "GLSL v" + GL2.glGetString(GL2::GL_SHADING_LANGUAGE_VERSION)
      puts "GLFW v" + GLFW.version_string
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
        sd.fetch(File.join(shader_path, 'text.vert')),
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
      reset_gl_flags
      create_input
      setup_default_shaders
      self
    end

    # Destroys the current window and cleans up.
    def shutdown
      @screen.close
      self
    end

    # Starts the main loop, terminate the loop using {#quit}
    def main
      @log.puts 'Starting main loop'
      clear_bits = GL2::GL_COLOR_BUFFER_BIT | GL2::GL_DEPTH_BUFFER_BIT
      until @screen.window.should_close?
        GL2.glClear clear_bits
        Audio.update
        @step.call self, @fps.restart
        @screen.window.title = sprintf "FPS: %d", @fps.fps
        @screen.window.swap_buffers
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
