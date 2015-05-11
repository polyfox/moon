module Moon
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
end
