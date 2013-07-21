class MusicActivityHud

  def initialize(x, y)
    @x = x 
    @y = y
    @spritesheet_8x8 = Spritesheet.new("resources/media_buttons_8x8.png", 8, 8)
  end

  def render
    @spritesheet_8x8.render(0, 0, 1) if Music.playing?
    @spritesheet_8x8.render(8, 0, 0) if Music.stopped?
    @spritesheet_8x8.render(16, 0, 2) if Music.finished?
  end

end