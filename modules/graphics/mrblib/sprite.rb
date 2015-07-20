module Moon
  class Sprite
    extend TypedAttributes
    include Shadable
    include OriginCoords

    attribute :opacity, Float
    attribute :angle,   Float
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
  end
end
