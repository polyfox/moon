class State::SpriteClipTest < State

  include Moon
  include Input

  def init
    super
    @sprite = Sprite.new("resources/media_buttons_96x96.png")
    @sprite.clip_rect = Rect.new(0, 0, 96, 96)
    w = @sprite.texture.width
    h = @sprite.texture.height
    @cols = @sprite.texture.width / 96
    @total = ((@sprite.texture.height / 96) * @cols).to_i
  end

  def render
    super
    @sprite.render
  end

  def update(delta)
    if Mouse.held?(Mouse::Buttons::BUTTON_1)
      @sprite.clip_rect = Rect.new(0, 0, 48, 36)
    elsif Mouse.held?(Mouse::Buttons::BUTTON_3)
      @sprite.clip_rect = Rect.new(0, 0, @sprite.texture.width*2, 36)
    else
      @sprite.clip_rect = nil
    end
    super delta
  end

end