class State::SpriteAngleTest < State

  def init
    super
    @size = Vector2.new(96, 96)
    @sprites = []
    @sprites << (Moon::Sprite.new "resources/media_buttons_#{@size.x.to_i}x#{@size.y.to_i}.png")
    @sprites[-1].clip_rect = Moon::Rect.new(@size.x*1, @size.y*1, @size.x, @size.y)
    @sprites << (Moon::Sprite.new "resources/media_buttons_#{@size.x.to_i}x#{@size.y.to_i}.png")
    @sprites[-1].clip_rect = Moon::Rect.new(@size.x*1, @size.y*2, @size.x, @size.y)
  end

  def update(delta)
    @sprites[0].x = (Moon::Screen.width - @size.x) / 2
    @sprites[0].y = (Moon::Screen.height - @size.y) / 2
    @sprites[0].angle += 1
    @sprites[0].ox = @size.x / 2

    @sprites[1].x = (Moon::Screen.width - @size.x) / 2
    @sprites[1].y = (Moon::Screen.height - @size.y) / 2
    @sprites[1].angle -= 1
    @sprites[1].ox = @size.x / 2
    @sprites[1].oy = @size.y / 2
    super delta
  end

  def render
    @sprites[0].render(0, 0, 0)
    @sprites[1].render(0, 0, 0)
    super
  end

end