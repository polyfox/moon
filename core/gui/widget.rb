#
# moon/core/gui/widget.rb
#
class Widget < Container

  attr_accessor :rx, :ry

  def initialize(parent, rx, ry, width, height)
    @parent = parent
    @rx = rx
    @ry = ry
    super(0, 0, width, height)
    @parent.widgets << self if @parent
  end

  def update
    self.x = @parent.x + rx
    self.y = @parent.y + ry
  end

  def render
  end

end
