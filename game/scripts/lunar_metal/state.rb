class LunarMetal::StateGame < State

  def init
    @game = LunarMetal::Game.new
    @hud = LunarMetal::Hud.new(@game)
    super
  end

  def update
    @game.update
    @hud.update
    super
  end

  def render
    super
    @hud.render
  end

end