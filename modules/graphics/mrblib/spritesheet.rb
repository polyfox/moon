module Moon
  class Spritesheet
    include Shadable

    attr_accessor :shader
    attr_reader :cell_count
    attr_reader :texture
    attr_reader :w
    attr_reader :h

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
      @shader = nil
      generate_buffers
    end
  end
end
