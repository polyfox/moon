module Moon
  # A ruby representation of an OpenGL VBO on the GPU. It will automatically
  # update the data as needed in one single upload to the GPU before the buffer
  # is being used. Note that it will automatically generate the acompanying IBO.
  #
  # This class is low-level and you shouldn't need to interact with it for most
  # usecases; unless you're implementing your own rendering.
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

    # @!method add_quad(rect, texture_rect, color)
    #   Pushes a quad into the buffer, the buffer's indices will be pushed as
    #   +[0, 1, 3, 2, 3, 1]+ automatically, to render correctly as a triangle strip.
    #   @param [Array<Integer>[4], Moon::Rect] rect the quad's dimensions
    #   @param [Array<Float>[4]] texture_rect a normalized rectangle for the texture coords, normally using +[0, 0, 1, 1]+ to cover the entire texture
    #   @param [Array<Float>[4], Vector4] color the color of the quad, normally +[1, 1, 1, 1]+, meaning pure white, the color will be multiplied into the quad's texture
    #   @return [self]

    # @!method add_quad_vertices(rect, texture_rect, color)
    #   Pushes a quad into the buffer, but the indices must be pushed manually.
    #   @param [Array<Integer>[4], Moon::Rect] rect the quad's dimensions
    #   @param [Array<Float>[4]] texture_rect a normalized rectangle for the texture coords, normally using +[0, 0, 1, 1]+ to cover the entire texture
    #   @param [Array<Float>[4], Vector4] color the color of the quad, normally +[1, 1, 1, 1]+, meaning pure white, the color will be multiplied into the quad's texture
    #   @return [self]

    # @see #push_back
    alias :push :push_back
  end
end
