module Moon
  # Utility class that offers an interface into the land of OpenGL.
  module OpenGL
    # @!group Drawing modes (primitives)
    LINES = GL2::GL_LINES
    POINTS = GL2::GL_POINTS
    TRIANGLES = GL2::GL_TRIANGLES
    TRIANGLE_STRIP = GL2::GL_TRIANGLE_STRIP
    TRIANGLE_FAN = GL2::GL_TRIANGLE_FAN
    # @!endgroup

    TEXTURE0 = GL2::GL_TEXTURE0

    # Checks the error flag for any errors. OpenGL will set it to a specific
    # error code if something goes wrong.
    #
    # We check for errors during engine initialization, as we set a whole bunch
    # of OpenGL flags at that point.
    #
    # @raise [Exception] if the error flag is set
    # @return [nil]
    def self.check_error!
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
      nil
    end

    # Resets Moon's required GL flags, since some external gems may change
    # them for their own evil ;3
    def self.reset_flags
      GL2.glDisable GL2::GL_DITHER
      check_error!
      GL2.glDisable GL2::GL_CULL_FACE
      check_error!
      GL2.glEnable GL2::GL_BLEND
      check_error!
      GL2.glBlendFunc GL2::GL_SRC_ALPHA, GL2::GL_ONE_MINUS_SRC_ALPHA
      check_error!
    end

    # Returns the guesstimated version of GLSL supported
    #
    # @return [String]
    def self.glsl_version
      GL2.glGetString(GL2::GL_SHADING_LANGUAGE_VERSION)
    end

    # The color with which the screen is cleared when we call glClear()
    # @param [Array<Integer>[4]] color_ary 4-int array representing RGBA
    def self.clear_color=(color_ary)
      GL2.glClearColor(*color_ary)
    end

    # Marks texture given an id as active
    #
    # @param [Integer] id
    def self.active_texture(id)
      GL2.glActiveTexture(id)
    end
  end
end
