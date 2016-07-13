module Moon
  # Sprite represents an image on screen. It has a texture and a set of
  # attributes. Multiple sprites can use the same texture to render different
  # variations of the same texture.
  class Sprite
    extend TypedAttributes
    include Shadable

    attribute :opacity, Float
    attribute :angle,   Numeric
    attribute :origin,  Vector2
    attribute :color,   Vector4
    attribute :tone,    Vector4
    attribute :shader,  Shader
    # call generate_buffers after assigning these
    attribute :clip_rect, Rect, nil
    attribute :texture, Texture

    # @param [Moon::Texture] texture
    def initialize texture
      set_texture texture
      @opacity = 1.0
      @angle = 0.0
      @origin = Vector2.new(0, 0)
      @color = Vector4.new(1.0, 1.0, 1.0, 1.0)
      @tone = Vector4.new(0.0, 0.0, 0.0, 1.0)

      @shader = self.class.default_shader
      @vbo = VertexBuffer.new(VertexBuffer::DYNAMIC_DRAW)
      @transform = Matrix4.new
      @rotation_matrix = Matrix4.new
      @mode = OpenGL::TRIANGLE_STRIP
      generate_buffers
    end

    alias :set_texture :texture=
    private :set_texture
    # @param [Moon::Texture] texture
    def texture= texture
      set_texture texture
      generate_buffers
    end

    alias :set_clip_rect :clip_rect=
    private :set_clip_rect
    # @param [Moon::Rect] rect
    def clip_rect= rect
      set_clip_rect rect
      generate_buffers
    end

    # Render the sprite at given coordinates.
    #
    # @param [Integer] x
    # @param [Integer] y
    # @param [Integer] z
    def render(x, y, z)
      @rotation_matrix.clear
      @rotation_matrix.rotate!(@angle, [0, 0, 1])
      @rotation_matrix.translate!(-@origin.x, -@origin.y, 0)
      @transform.clear
      @transform.translate!(x, y, z)
      transform = @transform * @rotation_matrix

      @shader.use
      @shader.set_uniform 'opacity', @opacity
      @shader.set_uniform 'color', @color
      @shader.set_uniform 'tone', @tone
      Renderer.instance.render(@shader, @vbo, @texture, transform, @mode)
    end
  end
end
