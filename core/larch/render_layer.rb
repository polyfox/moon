module Moon
  class RenderLayer < RenderContainer

    def initialize
      super
      @elements = []

      on :any do |event|
        case event
        when Moon::Input::MouseEvent
          subevent = event.dup
          subevent.position = screen_to_relative(subevent.position)
          @elements.each do |element|
            if element.pos_inside?(subevent.position)
              element.trigger subevent
            end
          end
        else
          @elements.each do |element|
            element.trigger event
          end
        end
      end
    end

    def width
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

    def height
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

    def each(&block)
      @elements.each(&block)
    end

    def add(element)
      @elements.push(element)
    end

    def remove(element)
      @elements.delete(element)
    end

    def update(delta)
      super delta
      @elements.each { |element| element.update(delta) }
    end

    def render(x=0, y=0, z=0)
      px, py, pz = *(@position + [x, y, z])
      @elements.each do |e|
        e.render px, py, pz if e.visible
      end
    end

  end
end
