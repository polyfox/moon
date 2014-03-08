class State::LunarMetal < State::Base

  def init
    @game = State::LunarMetal::Game.new
    @hud = State::LunarMetal::Hud.new(@game)
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