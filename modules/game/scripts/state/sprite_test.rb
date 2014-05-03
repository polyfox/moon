class State::SpriteTest < State

  def init
    @texture1 = Moon::Texture.new("resources/media_buttons_96x96.png")
    @texture2 = Moon::Texture.new("resources/media_buttons_32x32.png")
    @sprite = Moon::Sprite.new("resources/media_buttons_16x16.png")
    super
  end

  def render
    @sprite.render
    super
  end

  def update
    if @ticks % 10 == 0
      @sprite.texture = [@texture1, @texture2][(@ticks / 10) % 2]
    end
    super
  end

end