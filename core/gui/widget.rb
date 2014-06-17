module Moon
  class Widget < Container

    attr_accessor :rx, :ry

    def initialize(parent, rx, ry, width, height)
      @parent = parent
      @rx = rx
      @ry = ry
      super 0, 0, width, height
      @parent.widgets << self if @parent && @parent.respond_to?(:widgets)
    end

    def update(delta)
      self.x = @parent.x + rx
      self.y = @parent.y + ry
      super delta
    end

    def render
    end

  end
end
