module Moon
  class Renderer
    extend TypedAttributes
    attribute :projection_matrix, Matrix4
    attribute :view_matrix, Matrix4

    def initialize
      @view_matrix = Matrix4.new(1.0)
      @projection_matrix = Matrix4.new(1.0)
      @mvp_matrix = Matrix4.new
    end

    # Generate a model-view-projection matrix for the object we're about to
    # render.
    # @param [Matrix4] matrix4 the model matrix
    # @return [Matrix4] the resulting mvp_matrix
    private def apply_transform(matrix4)
      @mvp_matrix.clear
      @mvp_matrix.mul(@projection_matrix)
      @mvp_matrix.mul(@view_matrix)
      @mvp_matrix.mul(matrix4)
      @mvp_matrix
    end

    # Set the texture as the currently active texture in slot 0.
    # @param [Texture] texture texture to bind
    # @return [Integer] 0 (the id of the texture slot)
    private def bind_texture(texture)
      OpenGL.active_texture(OpenGL::TEXTURE0)
      texture.bind
      0 # id
    end

    # Binds shader and texture and renders the given vbo with transform
    #
    # @param [Moon::Shader] shader
    # @param [Moon::VertexBuffer] vbo
    # @param [Moon::Texture] texture
    # @param [Moon::Matrix4] transform
    # @param [Integer] mode opengl drawing mode eg. Moon::OpenGL::TRIANGLES
    # @param [Integer] index only used by spritesheet
    def render(shader, vbo, texture, transform, mode, index = 0)
      shader.set_uniform 'mvp_matrix', apply_transform(transform)
      shader.set_uniform 'tex0', bind_texture(texture) unless texture.nil?
      vbo.render mode, index
    end

    class << self
      attr_accessor :instance
    end
  end
end
