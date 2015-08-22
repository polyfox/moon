module Moon
  class Renderer
    attr_accessor :projection_matrix
    attr_accessor :view_matrix

    def initialize
      @view_matrix = Matrix4.new(1.0)
      @projection_matrix = Matrix4.new(1.0)
      @mvp_matrix = Matrix4.new
    end

    def apply_transform(matrix4)
      @mvp_matrix.clear
      @mvp_matrix.mul(@projection_matrix)
      @mvp_matrix.mul(@view_matrix)
      @mvp_matrix.mul(matrix4)
      @mvp_matrix
    end

    def bind_texture(texture)
      OpenGL.active_texture(OpenGL::TEXTURE0)
      texture.bind
      0 # id
    end

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
