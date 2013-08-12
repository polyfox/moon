#
# core/state_key_hold_test.rb
#   Press Spacebar and hold, watch the blocks render up
#   first row is the press count (currently does not work correctly)
#   second row is the release count (currently does not work correctly)
#   third is the repeat count
class State_KeyHoldTest < State

  class Hud_KeyHold

    attr_accessor :x, :y

    def initialize(device, key, cell_width=16, cell_height=16)
      @device = device
      @key = key
      @cell_width = cell_width
      @cell_height = cell_height
      @x = 0
      @y = 0
      init_spritesheet
    end

    def init_spritesheet
      filename = "resources/media_buttons_#{@cell_width}x#{@cell_height}.png"
      @spritesheet = Spritesheet.new(filename, @cell_width, @cell_height)
    end

    def render
      if @release
        @release.times do |i|
          @spritesheet.render(@x + @cell_width * i, @y, 0, 0)
        end
      end
      if @press
        @press.times do |i|
          @spritesheet.render(@x + @cell_width * i, @y + @cell_height, 0, 1)
        end
      end
      if @hold
        @hold.times do |i|
          @spritesheet.render(@x + @cell_width * i, @y + @cell_height * 2, 0, 2)
        end
      end
    end

    def update
      @press   = @device.pressed?(@key)
      @release = @device.released?(@key)
      @hold    = @device.repeated?(@key)
    end

  end

  def init
    @hud = Hud_KeyHold.new(Input, Input::Keys::SPACE, 8, 8)
    super
  end

  def render
    super
    @hud.render
  end

  def update
    @hud.update
    super
  end

end