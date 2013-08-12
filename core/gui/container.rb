# containers
class Container < Rectangle

  def initialize(x, y, width, height)
    super(x, y, width, height)
    on_resize
    on_move
  end

  # callbacks
  def on_move
  end

  def on_resize
  end

  def x=(new_x)
    super(new_x)
    on_move
  end

  def y=(new_y)
    super(new_y)
    on_move
  end

  def width=(new_width)
    super(new_width)
    on_resize
  end

  def height=(new_height)
    super(new_height)
    on_resize
  end

  def rx2
    rx + width
  end

  def ry2
    ry + height
  end

  def pos
    return [x, y]
  end

  def rpos
    return [rx, ry]
  end

  def size
    return [width, height]
  end

end