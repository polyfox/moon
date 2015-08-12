module Moon
  class Sprite
    extend TypedAttributes
    include Shadable

    attribute :opacity, Float
    attribute :angle,   Float
    attribute :origin,  Vector2
    attribute :color,   Vector4
    attribute :tone,    Vector4
    attribute :shader,  Shader
    # call generate_buffers after assigning these
    attribute :clip_rect, Rect, nil
    attribute :texture, Texture

    def initialize texture
      @texture = case texture
      when String
        Texture.new(texture)
      when Texture
        texture
      else
        raise TypeError, "wrong argument type #{texture.class} (expected Moon::Texture or String)"
      end

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
    def texture= texture
      set_texture texture
      generate_buffers
    end

    alias :set_clip_rect :clip_rect=
    def clip_rect= rect
      set_clip_rect rect
      generate_buffers
    end

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
