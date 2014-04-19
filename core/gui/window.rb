class Window < Container
  attr_accessor :widgets

  def initialize(x, y, width, height)
    super(x, y, width, height)

    @draggable = true
    @widgets = []

    # in a perfect scenario, the events would bubble up, not
    # down... here we just need to forward the events that
    # the dispatcher generates, and not those that are later
    # generated/detected (click)
    on :mouseup, :mousedown, :mousemove do |event|
      @widgets.each do |widget|
        widget.trigger event if Moon::Input::Mouse.in_rect?(widget)
      end
    end

    @windowskin = Moon::Spritesheet.new("resources/window.png", 16, 16)
  end

  def update
    @widgets.each(&:update.to_proc)
  end

  def render
    # render the windowskin (background)
    (width/16).to_i.times do |w|
      (height/16).to_i.times do |h|
        @windowskin.render(x+w*16, y+h*16, 1.0, 4)
      end
    end
    # edges (top/bottom)
    (width/16).to_i.times do |w|
      @windowskin.render(x+w*16, y, 1.0, 1)
      @windowskin.render(x+w*16, y+height-16, 1.0, 7)
    end
    # edges (left/right)
    (height/16).to_i.times do |h|
      @windowskin.render(x, y+h*16, 1.0, 3)
      @windowskin.render(x+width-16, y+h*16, 1.0, 5)
    end
    # corners
    @windowskin.render(x, y, 1.0, 0)
    @windowskin.render(x+width-16, y, 1.0, 2)
    @windowskin.render(x, y+height-16, 1.0, 6)
    @windowskin.render(x+width-16, y+height-16, 1.0, 8)

    # render widgets
    @widgets.each(&:render.to_proc)
  end
end
