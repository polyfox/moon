#=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
# Event based input handling
#------------------------------------------------------------
# When a button or a key gets pressed, a "press(ed)" event triggers exactly once (one frame). On release, a "release(d)" event gets triggered for one frame.
# (same as mousedown, mouseup).
# 
# When we detect a mousedown, followed by a mouseup, we've detected a "click", which triggers exactly once. A "dblclick"/"double click" is when one click
# event follows another, in a gap of less than 500ms.
# 
# mouseover - mouse is in the area of the object.
# mouseout - mouse is no longer in the area of the object
# mousemove - the mouse has moved
# mousewheel - the wheel has moved
# 
# if the mouse is pressed down (mousedown) and we get a mousemove event, that means we have a "drag" event.
# upon releasing the mouse (mouseup), we have a "drop" event.
# 
# "current click count" - consecutive clicks in a certain time frame
# 
# http://www.w3.org/TR/DOM-Level-3-Events/#event-type-click
# 
# For click detection, we need to match the mouseup and mousedown events, because for example if we mousedown on
# an object, then move the mouse away from the object and release the mouse, then click down and pull the mouse
# over the object, and release it, a click should not be triggered, because essentially, we just made 2 different
# clicks.
# To solve this, let's assign an ID to each mouse press/release pair, and store the last mousedown ID. When we
# get a mouseup, we check if the ID matches the mousedown ID. if it does, it's the same click, and we trigger
# a click event, else we just trash it.
# 
#----[ HTML5 ]----------------------------------------------
# 
# drag: fired frequently on a dragged element (which must define the *draggable* attribute.)
# dragstart: fired when the mouse is held down on an element and the movement starts.
# dragend: fired when the element is released.
# dragenter: fired when an element enters the displayed area of another one.
# dragleave: fired when an element exits the displayed area, as the inverse of dragenter.
# dragover: fired frequently when an element is over another.
# drop: fired when an element is released over another.
#-----------------------------------------------------------
# 
# 
# After GLFW_REPEAT triggers in, we 
#=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

class Event
  attr_reader :type, :altKey, :shiftKey, :ctrlKey, :metaKey
  attr_accessor :id # TEMP, proper initialization later

  def initialize(type)
    @type = type

    # modifier keys
    keys = Input::Keys
    button = Input::Mouse::Buttons::LEFT
    @altKey   = Input::Mouse.modded?(button, keys::MOD_ALT)
    @shiftKey = Input::Mouse.modded?(button, keys::MOD_SHIFT)
    @ctrlKey  = Input::Mouse.modded?(button, keys::MOD_CONTROL)
    @metaKey  = Input::Mouse.modded?(button, keys::MOD_SUPER)
  end
end

module Eventable
  def initialize
    @event_listeners = {:any => []}
  end

  # Adds a new event listener.
  # @param [Symbol] type The type to listen for..
  # @param [Proc] block The block we want to execute when we catch the type.
  def on type, &block
    @event_listeners[type] ||= []
    @event_listeners[type].push block
  end

  def trigger event
    event = Event.new(event) if !event.is_a?(Event) # TEMP
    @event_listeners[:any].each {|block| block.call(event) }
    return unless @event_listeners[event.type]
    @event_listeners[event.type].each do |block|
      block.call(event)
    end
  end
end

# Dirty, dirty, dirty temporary mouse wrapper
class EventDispatcher
  include Eventable

  def initialize
    super
    @last_state = nil
    @last_pos = [nil, nil]
    @clicks = 1
  end

  # LEFT CLICK HANDLING
  def update
    button = Input::Mouse::Buttons::LEFT

    if Input::Mouse.pressed?(button)
      if @last_state != :pressed # trigger only once
        # mousedown
        mousedown = Event.new(:mousedown)
        mousedown.id = (@clicks += 1)
        trigger mousedown
        @last_state = :pressed
      end

    else
      if @last_state != :released # trigger only once
        # mouseup
        mouseup = Event.new(:mouseup)
        mouseup.id = @clicks
        trigger mouseup
        @last_state = :released
      end
    end

    # mousemove
    current_pos = Input::Mouse.pos
    if @last_pos != current_pos
      trigger :mousemove
      @last_pos = current_pos
    end
  end
end

# Rename to "Window" or something later
class GUI_Window < Container
  attr_accessor :widgets

  def initialize(x, y, width, height)
    super(x, y, width, height)

    @draggable = true
    @widgets = []

    on :any do |event|
      @widgets.each {|widget| widget.trigger event }
    end

    @windowskin = Spritesheet.new("resources/window.png", 16, 16)
  end

  def update
    @widgets.each(&:update.to_proc)
  end

  def render

    #@widgets.each(&:render.to_proc)

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
  end
end


class Container < Rectangle
  attr_accessor :draggable

  def initialize(x, y, width, height)
    super(x, y, width, height)
    @event_listeners = {:any => []}

    @last_mousedown_id = 0 # dirty, dirty!

    # click event generation
    on :mousedown do |event|
      @last_mousedown_id = event.id
    end

    on :mouseup do |event|
      trigger :click if @last_mousedown_id == event.id
    end

    # dragging support
    @draggable = false

    on :mousedown do |event|
      # bonus: be able to specify a drag rectangle: 
      # the area where the user can click to drag
      # the window (useful if we only want it to
      # drag by the titlebar)
      
      # initiate dragging if @draggable = true
      if @draggable
        @dragging = true

        # store the relative offset of where the mouse
        # was clicked on the object, so we can accurately
        # set the new position
        @offset_x = Input::Mouse.x - self.x
        @offset_y = Input::Mouse.y - self.y
      end
    end

    # TODO: implement mousemove
    on :mousemove do |event|
      # if draggable, and we are dragging (the mouse is pressed down)

      # update the position, calculated off of
      # the mouse position and the relative offset
      # set on mousedown

      # don't forget to update the widget positions
      # too (refresh_position)
      # NOTE: at the moment the widget position is
      # updated in the update loop each cycle. Might
      # not be the most efficient thing to do.

      if @draggable && @dragging
        self.x = Input::Mouse.x - @offset_x
        self.y = Input::Mouse.y - @offset_y
      end
    end

    on :mouseup do |event|
      # disable dragging
      @dragging = false if @draggable
    end

    trigger :resize
    trigger :move
  end

  def x=(new_x)
    super(new_x)
    trigger :move
  end

  def y=(new_y)
    super(new_y)
    trigger :move
  end

  def width=(new_width)
    super(new_width)
    trigger :resize
  end

  def height=(new_height)
    super(new_height)
    trigger :resize
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

  # Adds a new event listener.
  # @param [Symbol] type The type to listen for..
  # @param [Proc] block The block we want to execute when we catch the type.
  def on type, &block
    @event_listeners[type] ||= []
    @event_listeners[type].push block
  end

  def trigger event
    event = Event.new(event) if !event.is_a?(Event) # TEMP
    @event_listeners[:any].each {|block| block.call(event) }
    return unless @event_listeners[event.type]
    @event_listeners[event.type].each do |block|
      block.call(event)
    end
  end

end

class Widget < Container

  attr_accessor :rx, :ry

  def initialize(parent, rx, ry, width, height)
    @parent = parent
    @rx = rx
    @ry = ry
    super(0, 0, width, height)
  end

  def update
    self.x = @parent.x + rx
    self.y = @parent.y + ry
=begin
    if Input::Mouse.in_rect?(self)
      call_event(:mouse_over)
      keys = Input::Keys
      button = Input::Mouse::Buttons::LEFT
      if Input::Mouse.triggered?(button)
        call_event(:mouse_focus)
      end
    end
=end
  end

end

class State_Mouse_Events < State

  def init
    @handler = EventDispatcher.new
    @window = GUI_Window.new(32,32,128,128)
    area = Widget.new(@window, 0,0,64,64)

    area.on :click do |event|
      puts "area clicked!" if Input::Mouse.in_rect? area
    end

    @window.on :click do |event|
      puts "window clicked with ALT!" if event.altKey
    end

    @window.widgets << area

    @handler.on :any do |event|
      if Input::Mouse.in_rect? @window
        @window.trigger event
      end
    end
  end

  def update
    @handler.update
    @window.update
  end

  def render
    @window.render
  end
end