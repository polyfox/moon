module Moon
  class Spritesheet
    include Shadable

    attr_reader :texture, :tile_width, :tile_height, :total_sprites

    def initialize texture, tile_width, tile_height
      @texture = case texture
      when String
        Texture.new(texture)
      when Texture
        texture
      else
        raise TypeError, "wrong class, expected Moon::Texture"
      end
      
      @w  = tile_width
      @h = tile_height
      @cell_count = 0
      generate_buffers
    end
  end
end
