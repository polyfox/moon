class State::SpriteColorTest < State

  include Moon

  def init
    super
    @sprite = Sprite.new("resources/media_buttons_96x96.png")
    @sprite.color = Vector4.new(0.6, 0.5, 0.3, 1.0)
  end

  def render
    super
    @sprite.render 0, 0, 0
  end

end