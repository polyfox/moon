module Moon
  class RenderContainer

    include ScreenElement  # Moon::Core
    include Transitionable # Moon::Core
    include Eventable      # Moon::Core
    include Enumerable

    @@container_id = 0

    attr_reader :id
    attr_accessor :parent
    attr_accessor :position
    attr_accessor :visible
    attr_writer :width
    attr_writer :height

    def initialize
      @id = @@container_id += 1
      @parent = nil
      @elements = []
      @visible = true
      @position = Vector3.new
      init_eventable
      init_events
    end

    def init_events
      on :any do |_, event|
        #puts "[#{self}] #{event}"
        case event
        when Moon::Input::MouseEvent
          subevent = event.dup
          subevent.position = screen_to_relative(subevent.position)
          each do |element|
            if element.pos_inside?(subevent.position)
              element.trigger subevent
            end
          end
        else
          each do |element|
            element.trigger event
          end
        end
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

    def render_elements(x, y, z)
      @elements.each do |e|
        e.render x, y, z if e.visible
      end
    end

    def render(x=0, y=0, z=0)
      px, py, pz = *(@position + [x, y, z])
      render_elements(px, py, pz)
      self
    end

  end
end
