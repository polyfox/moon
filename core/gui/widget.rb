#
# moon/core/gui/widget.rb
#
class Widget < Container

  attr_accessor :rx, :ry

  def initialize(rx, ry, width, height)
    @rx = rx
    @ry = ry
    super(0, 0, width, height)
  end

  def on_event(event, &func)
    @events ||= {}
    @events[event] = func
  end

  def event_list
    return @events.keys
  end

  def event?(symbol)
    return !!@events[symbol]
  end

  def call_event(symbol)
    @events[symbol].call(self) if event?(symbol)
  end

  def update
    if Input::Mouse.in_rect?(self)
      call_event(:mouse_over)
      keys = Input::Keys
      button = Input::Mouse::Buttons::LEFT
      if Input::Mouse.triggered?(button)
        if    Input::Mouse.modded?(button, keys::MOD_SHIFT)
          call_event(:mouse_focus_shift)
        elsif Input::Mouse.modded?(button, keys::MOD_CONTROL)
          call_event(:mouse_focus_control)
        elsif Input::Mouse.modded?(button, keys::MOD_ALT)
          call_event(:mouse_focus_alt)
        elsif Input::Mouse.modded?(button, keys::MOD_SUPER)
          call_event(:mouse_focus_super)
        else
          call_event(:mouse_focus_no_mod)
        end
        call_event(:mouse_focus)
      end
    end
  end

  def refresh_position(x, y)
    self.x = x + rx
    self.y = y + ry
  end

end