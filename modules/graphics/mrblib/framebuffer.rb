module Moon
  class Framebuffer

    def initialize
      @texture = Texture.new(600, 800)
      @vbo = VertexBuffer.new(VertexBuffer::STATIC_DRAW)
      @shader = Shader.load('todo', 'todo')

      # TODO: generate the 0->1 vertices for a quad that covers entire screen

      # TODO: handle window resizes, texture needs to be resized

      # Depth buffer RBO
      rbo_depth = GL2::glGenRenderbuffers(1).first
      GL2::glBindRenderbuffer(GL2::GL_RENDERBUFFER, rbo_depth)
      GL2::glRenderbufferStorage(GL2::GL_RENDERBUFFER, GL2::GL_DEPTH_COMPONENT16, screen_width, screen_height)
      GL2::glBindRenderbuffer(GL2::GL_RENDERBUFFER, 0)

      # Framebuffer to link everything together
      @fbo = GL2::glGenFramebuffers(1).first
      GL2::glBindFramebuffer(GL2::GL_FRAMEBUFFER, @fbo)

      # attach the texture to FBO color attachment point
      GL2::glFramebufferTexture2D(
        GL2::GL_FRAMEBUFFER,
        GL2::GL_COLOR_ATTACHMENT0,
        GL2::GL_TEXTURE_2D,
        @texture.id,
        0
      )

      # attach the renderbuffer to depth attachment point
      GL2::glFramebufferRenderbuffer(
        GL2::GL_FRAMEBUFFER,
        GL2::GL_DEPTH_ATTACHMENT,
        GL2::GL_RENDERBUFFER,
        rbo_depth
      )

      if ((status = GL2::glCheckFramebufferStatus(GL2::GL_FRAMEBUFFER)) != GL2::GL_FRAMEBUFFER_COMPLETE)
        fprintf(stderr, "glCheckFramebufferStatus: error %p", status)
        return 0
      end
      glBindFramebuffer(GL_FRAMEBUFFER, 0)
    end

    def bind
      # set rendering destination to FBO
      GL2::glBindFramebuffer(GL2::GL_FRAMEBUFFER, @fbo)

      # clear buffers
      clear_bits = GL2::GL_COLOR_BUFFER_BIT | GL2::GL_DEPTH_BUFFER_BIT
      GL2.glClear clear_bits

      # draw a scene to a texture directly
      yield

      # unbind FBO
      GL2::glBindFramebuffer(GL2::GL_FRAMEBUFFER, 0)
    end

    def render
      @shader.use
      @shader.set_uniform 'tex0', Renderer.instance.bind_texture(@texture)
      glDisable(GL_DEPTH_TEST)
      @vbo.render OpenGL::TRIANGLE_STRIP
    end

    def terminate
      # TODO: delete FBO & RBO
    end

  end
end
