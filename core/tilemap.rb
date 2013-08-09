#
# moon/core/tilemap.rb
#
# This may need to be rewritten in C/++
class Tilemap

  attr_accessor :tileset # Spritesheet
  attr_accessor :data    # DataMatrix

  ##
  # initialize
  def initialize
    @tileset = nil
    @data = nil
    @loop_data = false
  end

  ##
  # render(int x, int y, int z, int ox, int oy, int width, int height)
  #   (x), (y), (z)
  #     is the position (in pixels), for the Tilemap to be rendered
  #   (ox), (oy), (oz)
  #     is the data offset (where should the Tilemap#data start from)
  #   (width), (height), (layers)
  #     is the number of cells to render, and the number of layers to be rendered
  def render(x, y, z, ox, oy, oz, width, height, layers)
    return unless @data
    return unless @tileset
    cell_width  = @tileset.cell_width
    cell_height = @tileset.cell_height
    # we loop by layer
    layers.times do |l|
      dz = l + oz # offset data z index
      if @loop_data
        dz %= @data.zsize
      else
        next if dz < 0 || dz >= @data.zsize
      end
      # and then by row
      height.times do |i|
        dy = i + oy # offset data y index
        if @loop_data
          dy %= @data.ysize
        else
          next if dy < 0 || dy >= @data.ysize
        end
        # and then render by cell
        width.times do |j|
          dx = j + ox # offset data x index
          if @loop_data
            dx %= @data.xsize
          else
            next if dx < 0 || dx >= @data.xsize
          end
          tile_id = @data[dx, dy, dz]
          next if tile_id < 0 # if -1 or less, then its a negative tile and therefore should not be rendered
          # TODO add a @data_zmap, to have custom z heights in rendering
          @tileset.render(x + j * cell_width, y + i * cell_height, z, tile_id)
        end
      end
    end
  end

end