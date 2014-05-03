#
# core/music_activity_hud.rb
#   Shows the active state of the Music class
class MusicActivityHud

  attr_accessor :x
  attr_accessor :y
  attr_accessor :z

  def initialize(x, y, z)
    # later we can have different music objects with different huds
    @x = x
    @y = y
    @z = z
    @music = Moon::Music
    @spritesheet = Cache.spritesheet("media_buttons_8x8")
    @cell_width = @spritesheet.cell_width
  end

  def render
    @spritesheet.render(@x, @y, @z, 1) if @music.playing?
    @spritesheet.render(@x + @cell_width, @y, @z, 0) if @music.stopped?
    @spritesheet.render(@x + @cell_width * 2, @y, @z, 2) if @music.finished?
  end

end