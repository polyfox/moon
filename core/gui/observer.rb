# Observer, watches the input for us, and distributes events to
# subscribed objects.
#
# Warning, do not include it

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

# Dirty, dirty, dirty temporary mouse wrapper
=begin
class Observer
  include Eventable

  def initialize
    super
    init_eventable
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
=end
