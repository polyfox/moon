module Moon
  class Spritesheet
    extend TypedAttributes
    include Shadable

    attribute :shader,  Shader
    attr_reader :cell_count
    attr_reader :texture
    attr_reader :w, :h

    def initialize texture, tile_width, tile_height
      @texture = case texture
      when String
        Texture.new(texture)
      when Texture
        texture
      else
        raise TypeError, "wrong argument type #{texture.class} (expected Moon::Texture or String)"
      end
      @w = tile_width
      @h = tile_height
      if 0 >= @w
        raise ArgumentError, "@w must be greater than 0"
      elsif 0 >= @h
        raise ArgumentError, "@h must be greater than 0"
      end
      @cell_count = 0
      @shader = self.class.default_shader
      @vbo = VertexBuffer.new(VertexBuffer::STATIC_DRAW)
      @transform = Matrix4.new
      @rotation_matrix = Matrix4.new
      @mode = OpenGL::TRIANGLE_STRIP
      generate_buffers
    end

    def render(x, y, z, index, options = {})
      angle = options.fetch(:angle, 0.0)
      origin = options.fetch(:origin) { Moon::Vector2.new(0, 0) }
      opacity = options.fetch(:opacity, 1.0)
      color = options.fetch(:color, 1.0) { Moon::Vector4.new(1, 1, 1, 1) }
      tone = options.fetch(:tone, 1.0) { Moon::Vector4.new(0, 0, 0, 1) }

      @rotation_matrix.clear
      @rotation_matrix.rotate!(angle, [0, 0, 1])
      @rotation_matrix.translate!(-origin.x, -origin.y, 0)
      @transform.clear
      @transform.translate!(x, y, z)
      transform = @transform * @rotation_matrix

      @shader.use
      @shader.set_uniform 'opacity', opacity
      @shader.set_uniform 'color', color
      @shader.set_uniform 'tone', tone
      Renderer.instance.render(@shader, @vbo, @texture, transform, @mode, index)
    end
  end
end
