module Moon
  class RenderContainer

    include ScreenElement  # Moon::Core
    include Eventable

    @@container_id = 0

    attr_reader :id
    attr_accessor :position

    def initialize
      @visible = true
      @position = Vector3.new
      @id = @@container_id += 1
      init_eventable
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
      0
    end

    def height
      0
    end

    def x2
      x + width
    end

    def y2
      y + height
    end

    def update(delta)
      update_transitions delta
    end

    def render(x=0, y=0, z=0)
      self
    end

  end
end
