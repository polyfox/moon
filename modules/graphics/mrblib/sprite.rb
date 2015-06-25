module Moon
  class Sprite
    include Shadable
    
    # TODO: validate types on assignment here, create a mixin
    attr_accessor :opacity, :angle, :origin, :color, :tone, :shader
    # call generate_buffers after assigning these
    attr_reader :clip_rect, :texture

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
      @color = Vector4(1.0, 1.0, 1.0, 1.0)
      @tone = Vector4(1.0, 1.0, 1.0, 1.0)

      @shader = self.class.default_shader
      @vbo = VertexBuffer.new(VertexBuffer::DYNAMIC_DRAW)
      generate_buffers
    end
    
    def texture= texture
      if !texture.is_a? Moon::Texture
        raise TypeError, "wrong argument type #{texture.class} (expected Moon::Texture)"
      end
      @texture = texture
      generate_buffers
    end

    def clip_rect rect
      # TODO: validate
      @clip_rect = rect
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
