module Moon
  class VertexBuffer
    # @!group Buffer types
    DYNAMIC_DRAW = GL2::GL_DYNAMIC_DRAW
    STATIC_DRAW  = GL2::GL_STATIC_DRAW
    STREAM_DRAW  = GL2::GL_STREAM_DRAW
    # @!endgroup

    QUAD_INDICES = [0, 1, 3, 2, 3, 1]

    alias :push :push_back
  end
end
