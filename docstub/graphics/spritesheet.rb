module Moon
  # Spritesheet is a specialized class for rendering cell based sprites.
  class Spritesheet
    # @param [String, Texture] obj
    # @param [Integer] cell_w
    # @param [Integer] cell_h
    def initialize(obj, cell_w, cell_h)
    end

    # @param [Float] x
    # @param [Float] y
    # @param [Float] z
    def render(x, y, z, index)
    end

    # @return [Texture]
    def texture
    end

    # @return [Integer]
    def cell_count
    end

    # @return [Integer]
    def w
    end

    # @return [Integer]
    def h
    end

    # dreprecated use w instead
    alias :cell_w :w

    # dreprecated use h instead
    alias :cell_h :h
  end
end
