# small data requirement
# eventually this will be implemented in C++ 
class Rectangle

  attr_accessor :x, :y, :width, :height

  def initialize(x, y, width, height)
    @x      = x
    @y      = y
    @width  = width
    @height = height
  end

  def to_a
    [@x, @y, @width, @height]
  end

end