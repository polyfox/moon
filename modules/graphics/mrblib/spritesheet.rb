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
      @cell_count = 0
      @shader = self.class.default_shader
      @vbo = VertexBuffer.new(VertexBuffer::STATIC_DRAW)
      generate_buffers
    end
  end
end
