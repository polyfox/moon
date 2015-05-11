module Moon
  # Basic rendering class for rendering Textures
  class Sprite
    # @return [Float]
    attr_accessor :angle
    # @return [Float]
    attr_accessor :opacity
    # @return [Float]
    attr_accessor :ox
    # @return [Float]
    attr_accessor :oy
    # @return [Vector4]
    attr_accessor :color
    # @return [Vector4]
    attr_accessor :tone
    # @return [Texture]
    attr_accessor :texture
    # @return [Rect]
    attr_accessor :clip_rect

    # @param [String, Texture] obj
    def initialize(obj)
    end

    # @param [Float] x
    # @param [Float] y
    # @param [Float] z
    def render(x, y, z)
    end
  end
end
