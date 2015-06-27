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
      generate_buffers
    end

    def texture= texture
      super
      generate_buffers
    end

    def clip_rect= rect
      super
      generate_buffers
    end

    # TODO: make sure origin is never nil, but Vec(0,0)
    def ox
      origin.x
    end

    def oy
      origin.y
    end

    def ox= x
      origin.x = x
    end

    def oy= y
      origin.y = y
    end
  end
end
