#
# core/state_music_activity.rb
#   Shows the current state of the Music class, whether it is currently
#   playing?, stopped? or finished?
class State_MusicActivity < State
  
  def init
    @music_hud = MusicActivityHud.new(0, 0)
    #@music = Music.new
    Music.play("resources/CamelsNommingHay.ogg", "ogg")
    super
  end

  def render
    super
    @music_hud.render
  end

  def update
    render
    super
  end

end