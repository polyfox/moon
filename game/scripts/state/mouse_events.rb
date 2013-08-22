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
#=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

class Event
  attr_reader :type, :altKey, :shiftKey, :ctrlKey, :metaKey
  attr_accessor :id # TEMP, proper initialization later

  def initialize(type)
    @type = type

    # modifier keys
    keys = Moon::Input::Keyboard::Keys
    button = Moon::Input::Mouse::Buttons::LEFT
    @altKey   = Moon::Input::Mouse.modded?(button, keys::MOD_ALT)
    @shiftKey = Moon::Input::Mouse.modded?(button, keys::MOD_SHIFT)
    @ctrlKey  = Moon::Input::Mouse.modded?(button, keys::MOD_CONTROL)
    @metaKey  = Moon::Input::Mouse.modded?(button, keys::MOD_SUPER)
  end
end

module Eventable
  def initialize
    @event_listeners = {:any => []}
  end

  # Adds a new event listener.
  # @param [Symbol] types The types to listen for..
  # @param [Proc] block The block we want to execute when we catch the type.
  def on *types, &block
    types.each do |type|
      @event_listeners[type] ||= []
      @event_listeners[type].push block
    end
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
    button = Moon::Input::Mouse::Buttons::LEFT

    if Moon::Input::Mouse.pressed?(button)
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
    current_pos = Moon::Input::Mouse.pos
    if @last_pos != current_pos
      trigger :mousemove
      @last_pos = current_pos
    end
  end
end

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
      @widgets.each {|widget|
        widget.trigger event if Moon::Input::Mouse.in_rect?(widget)
      }
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

class State_Mouse_Events < State

  def init
    @handler = EventDispatcher.new
    @window = Window.new(32,32,128,128)

    button = Button.new(@window, 0, 0, "Test") { puts "button click'd!" }

    @window.on :click do |event|
      puts "window clicked with ALT!" if event.altKey
    end

    @window.on :dblclick do |event|
      puts "double clicked!"
    end

    @handler.on :any do |event|
      if Moon::Input::Mouse.in_rect? @window
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