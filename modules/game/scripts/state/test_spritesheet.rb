class State::TestSpritesheet < State

  include Moon

  def random_bi
    rand(2) == 0 ? -1 : 1
  end

  def random_color
    Vector4.new rand(), rand(), rand(), 1.0
  end

  def random_tone
    Vector4.new random_bi * rand(), random_bi * rand(), random_bi * rand()
  end

  def init
    super
    @ss = Spritesheet.new "resources/media_buttons_16x16.png", 16, 16
    @color = random_color
    @tone = random_tone
  end

  def update(delta)
    super delta
  end

  def render
    super
    cw = @ss.cell_width
    ch = @ss.cell_height
    @ss.render cw*0, 0, 0, 2
    @ss.render cw*1, 0, 0, 2, opacity: 0.5
    @ss.render cw*2, 0, 0, 2, angle: 45, ox: cw / 2, oy: ch / 2
    @ss.render cw*3, 0, 0, 2, color: @color
    @ss.render cw*4, 0, 0, 2, tone: @tone
  end

end