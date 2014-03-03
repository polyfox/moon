#
# core/state_music_activity.rb
#   Shows the current state of the Music class, whether it is currently
#   playing?, stopped? or finished?
#   The icons are a bit small, and placed in the top-left corner of the screen.
class State::MusicActivity < State

  def init
    @music_hud = MusicActivityHud.new(0, 0, 0)
    #@music = Music.new
    Moon::Music.play("resources/CamelsNommingHay.ogg", "ogg")
    super
  end

  def render
    super
    @music_hud.render
  end

  def update
    super
  end

end