#
# core/state_key_hold_test.rb
#   Press Spacebar and hold, watch the blocks render up
#   first row is the press count (currently does not work correctly)
#   second row is the release count (currently does not work correctly)
#   third is the repeat count
class State_KeyHoldTest < State

  def init
    @cell_width = 16
    @spritesheet = Spritesheet.new("resources/media_buttons_16x16.png", 16, 16)
    super
  end

  def render
    super
    if @release
      @release.times do |i|
        @spritesheet.render(@cell_width * i, 0, 0)
      end
    end
    if @press
      @press.times do |i|
        @spritesheet.render(@cell_width * i, @cell_width, 0)
      end
    end
    if @hold 
      @hold.times do |i|
        @spritesheet.render(@cell_width * i, @cell_width * 2, i % 12)
      end
    end
  end

  def update
    @press = Input.pressed?(Input::Keys::SPACE)
    @release = Input.released?(Input::Keys::SPACE)
    @hold = Input.repeated?(Input::Keys::SPACE)
    render
    super
  end

end