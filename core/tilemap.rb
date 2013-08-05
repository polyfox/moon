#
# moon/core/tilemap.rb
#
# This may need to be rewritten in C/++
class Tilemap

  attr_accessor :tileset # Spritesheet
  attr_accessor :data    # Table

  ##
  # initialize
  def initialize
    @tileset = nil
    @data = nil
    @loop_data = false
  end

  ##
  # render(int x, int y, int z, int ox, int oy, int width, int height)
  #   (x), (y), (z) are the positions in which this Tilemap should be render on screen
  #   (ox), (oy) is the data offset (where should the tilemap#data start from)
  #   (width), (height) is the render size in cells of the tilemap
  def render(x, y, z, ox, oy, width, height)
    return unless @data
    return unless @tileset
    cell_width  = @tileset.cell_width
    cell_height = @tileset.cell_height
    for i in 0...height
      for j in 0...width
        dx = j + ox
        dy = i + oy
        if @loop_data
          dx %= @data.xsize
          dy %= @data.ysize
        else
          next if dx < 0 || dy < 0 || dx >= @data.xsize || dy >= @data.ysize
        end
        tile_id = @data[dx, dy]
        @tileset.render(x + j * cell_width, y + i * cell_height, z, tile_id)
      end
    end
  end

end