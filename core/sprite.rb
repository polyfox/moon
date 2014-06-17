module Moon
  class Sprite

    def width
      clip_rect ? clip_rect.width : (texture ? texture.width : 0)
    end

    def height
      clip_rect ? clip_rect.height : (texture ? texture.height : 0)
    end

  end
end
