module Moon # :nodoc:
  # Class for loading and rendering font files
  # Supported formats are .ttf and possibly .otf
  class Font
    # @param [String] filename
    # @param [Integer] size
    def initialize(filename, size)
    end

    # @param [Float] x
    # @param [Float] y
    # @param [Float] z
    # @param [String] text
    # @param [Vector4] color
    # @param [Hash<Symbol, Object>] options
    # @option options [Float] :outline
    # @option options [Vector4] :outline_color
    # @option options [Transform] :transform
    def render(x, y, z, text, color = nil, options = {})
    end
  end

  # Shader object
  class Shader
    # @return [Transform]
    def self.projection_matrix
    end

    # @param [Transform] mat
    def self.projection_matrix=(mat)
    end
  end

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

  # A storage class for pixel data on the GPU.
  class Texture
    # @return [Float]
    attr_reader :w
    # @return [Float]
    attr_reader :h

    # @param [String] filename
    def initialize(filename)
    end
  end
end
