# Inspired by
# http://dev.chromium.org/developers/design-documents/aura
#
module Moon
  class RenderContainer
    include ScreenElement  # Moon::Core
    include Transitionable # Moon::Core
    include Eventable      # Moon::Core
    include Renderable     # Moon::Core
    include Enumerable

    @@container_id = 0

    attr_reader :id
    attr_reader :elements
    attr_accessor :parent
    attr_accessor :position
    attr_accessor :visible

    def initialize
      @id = @@container_id += 1
      @parent = nil
      @elements = []
      @visible = true
      @position = Vector3.new
      init_eventable
      init_elements
      init_events

      trigger :resize
    end

    def init_elements
      #
    end

    def init_events
      # generic event passing callback
      # this callback will trigger the passed event in the children elements
      # Input::MouseEvent are handled specially, since it requires adjusting
      # the position of the event
      on :any do |event|
        each do |element|
          element.trigger event
        end
      end

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

      on :mouseup do |event, _|
        # disable dragging
        @dragging = false if @draggable
      end
    end

    def input_trigger(event)
      if event.is_a?(Moon::Input::MouseEvent)
        trigger(event) if pos_inside?(event.position)
      else
        trigger(event)
      end
    end

    def x
      @position.x
    end

    def y
      @position.y
    end

    def z
      @position.z
    end

    def width
      @width ||= begin
        x = 0
        x2 = 0
        @elements.each do |e|
          ex = e.x
          ex2 = ex + e.width
          x = ex if ex < x
          x2 = ex2 if ex2 > x2
        end
        x2 - x
      end
    end

    def width=(width)
      @width = width
      trigger :resize
    end

    def height
      @height ||= begin
        y = 0
        y2 = 0
        @elements.each do |e|
          ey = e.y
          ey2 = ey + e.height
          y = ey if ey < y
          y2 = ey2 if ey2 > y2
        end
        y2 - y
      end
    end

    def height=(height)
      @height = height
      trigger :resize
    end

    def x2
      x + width
    end

    def y2
      y + height
    end

    def move(x, y, z=self.z)
      @position.set(x, y, z)
      self
    end

    def resize(w, h)
      @width, @height = w, h
      self
    end

    def each(&block)
      @elements.each(&block)
    end

    def add(element)
      @elements.push(element)
      element.parent = self
      element
    end

    def remove(element)
      @elements.delete(element)
      element.parent = nil
      element
    end

    def update_elements(delta)
      @elements.each { |element| element.update(delta) }
    end

    def update(delta)
      update_elements(delta)
      update_transitions(delta)
    end

    def render_elements(x, y, z, options={})
      @elements.each do |e|
        e.render x, y, z if e.visible
      end
    end

    def render(x=0, y=0, z=0, options={})
      px, py, pz = *(@position + [x, y, z])
      render_elements(px, py, pz, options)
      super
    end

    protected :elements
    private :init_elements
    private :init_events
    private :init_eventable
  end
end
