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
    super
  end

end