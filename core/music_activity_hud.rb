#
# core/music_activity_hud.rb
#   Shows the active state of the Music class
class MusicActivityHud

  def initialize(x, y)
    @music = Music # later we can have different music objects with different huds
    @x = x
    @y = y
    @spritesheet_8x8 = Spritesheet.new("resources/media_buttons_8x8.png", 8, 8)
    @cell_width = @spritesheet_8x8.cell_width
  end

  def render
    @spritesheet_8x8.render(0, 0, 0, 1) if @music.playing?
    @spritesheet_8x8.render(@cell_width, 0, 0, 0) if @music.stopped?
    @spritesheet_8x8.render(@cell_width * 2, 0, 0, 2) if @music.finished?
  end

end