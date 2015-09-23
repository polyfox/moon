module Moon
  class Spritesheet
    extend TypedAttributes
    include Shadable

    # default origin
    # @return [Moon::Vector2]
    ZERO_ORIGIN = Moon::Vector2.new(0, 0)
    # default shader color
    # @return [Moon::Vector4]
    WHITE = Moon::Vector4.new(1, 1, 1, 1)
    # default shader tone
    # @return [Moon::Vector4]
    BLACK = Moon::Vector4.new(0, 0, 0, 1)

    attribute :shader,  Shader
    attr_reader :cell_count
    attribute :texture, Texture
    attr_reader :w, :h

    private :texture=

    # @param [Texture] texture
    # @param [Integer] tile_width
    # @param [Integer] tile_height
    def initialize texture, tile_width, tile_height
      self.texture = texture
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

    # @param [Integer] x
    # @param [Integer] y
    # @param [Integer] z
    # @param [Integer] index
    # @param [Hash<Symbol, Object>] options
    # @option options [Float] angle
    # @option options [Vector2] origin
    # @option options [Float] opacity
    # @option options [Vector4] color
    # @option options [Vector4] tone
    def render(x, y, z, index, options = {})
      angle = options.fetch(:angle, 0.0)
      origin = options.fetch(:origin, ZERO_ORIGIN)
      opacity = options.fetch(:opacity, 1.0)
      color = options.fetch(:color, WHITE)
      tone = options.fetch(:tone, BLACK)

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
      Renderer.instance.render(@shader, @vbo, @texture, transform, @mode, index * 4)
    end
  end
end
