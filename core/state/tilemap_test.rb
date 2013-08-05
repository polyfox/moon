#
# core/state/tilemap_test.rb
#
class State_TilemapTest < State

  ##
  # init
  def init
    super
    @tilemap = Tilemap.new
    @tilemap.tileset = Spritesheet.new("resources/hyptosis_tile-art-batch-1.png", 32, 32)
    @tilemap.data = Table.new((Window.width / 32).to_i, (Window.height / 32).to_i)
    @tilemap.data.map! do |n, x, y|
      rand(30 * 30) # (Spritesheet#cols * Spritesheet#rows)
    end
  end

  def render
    super
    @tilemap.render(0, 0, 0, 0, 0, @tilemap.data.xsize, @tilemap.data.ysize)
  end

  def update
    # nothing to do here folks, move along
    super
  end

end
# TODO:
# Spritesheet#cols -> returns the total number of columns in the Spritesheet
# Spritesheet#rows -> returns the total number of rows in the Spritesheet
# Spritesheet#size -> returns the total number of cells in the Spritesheet (or just #cols * #rows)