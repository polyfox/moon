require 'core/gui/observer'

class Container < Moon::Rect

  attr_accessor :draggable
  attr_accessor :widgets
  attr_accessor :rx
  attr_accessor :ry

  def initialize(x, y, width, height)
    #@rect = Moon::Rect.new x, y, width, height
    super x, y, width, height
    @rx, @ry = 0, 0

    @widgets ||= []

    @event_listeners = {:any => []}

    @last_mousedown_id = 0 # dirty, dirty!
    @last_click_at = 0.0

    # click event generation
    on :mousedown do |event|
      @last_mousedown_id = event.id
    end

    on :mouseup do |event|
      trigger :click if @last_mousedown_id == event.id
    end

    # double clicks (click distance was max 500ms)
    on :click do |event|
      now = Moon::Screen.uptime
      if now - @last_click_at < 0.500
        trigger :dblclick
        # reset the distance, so we can't trigger
        #consecutive double clicks with a single click
        @last_click_at = 0.0
      else
        @last_click_at = now
      end
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
        @offset_x = Moon::Input::Mouse.x - self.x
        @offset_y = Moon::Input::Mouse.y - self.y
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
        self.x = Moon::Input::Mouse.x - @offset_x
        self.y = Moon::Input::Mouse.y - @offset_y
      end
    end

    on :mouseup do |event|
      # disable dragging
      @dragging = false if @draggable
    end

    trigger :resize
    trigger :move
  end

  #def x
  #  @rect.x
  #end
  #def y
  #  @rect.y
  #end
  #def width
  #  @rect.width
  #end
  #def height
  #  @rect.height
  #end

  def x=(new_x)
    super(new_x)
    #@rect.x = new_x
    trigger :move
  end

  def y=(new_y)
    super(new_y)
    #@rect.y = new_y
    trigger :move
  end

  def width=(new_width)
    super(new_width)
    #@rect.width = new_width
    trigger :resize
  end

  def height=(new_height)
    super(new_height)
    #@rect.height = new_height
    trigger :resize
  end

  def rx2
    rx + width
  end

  def ry2
    ry + width
  end

  def pos
    Vector2.new x, y
  end

  def size
    Vector2.new width, height
  end

  # Adds a new event listener.
  # @param [Symbol] types The types to listen for..
  # @param [Proc] block The block we want to execute when we catch the type.
  def on *types, &block
    types.each do |type|
      (@event_listeners[type] ||= []).push block
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
