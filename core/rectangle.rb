# small data requirement
# eventually this will be implemented in C++
class Moon::Rect

  def to_a
    [x, y, width, height]
  end

  def x2
    x + width
  end

  def y2
    y + height
  end

end