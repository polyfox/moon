module Moon
  # A ruby representation of an OpenGL VBO on the GPU. It will automatically
  # update the data as needed in one single upload to the GPU before the buffer
  # is being used. Note that it will automatically generate the acompanying IBO.
  class VertexBuffer
    # @!group Buffer types
    # Buffer will be dynamically updated.
    DYNAMIC_DRAW = GL2::GL_DYNAMIC_DRAW
    # Buffer will be static and we will never (or rarely update it).
    STATIC_DRAW  = GL2::GL_STATIC_DRAW
    # Buffer will be "streamed" to on every frame.
    STREAM_DRAW  = GL2::GL_STREAM_DRAW
    # @!endgroup

    # The default order of indices to draw a quad using a triangle strip.
    QUAD_INDICES = [0, 1, 3, 2, 3, 1]

    alias :push :push_back
  end
end
