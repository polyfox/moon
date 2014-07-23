module Moon
  class Widget < RenderContainer
    attr_accessor :focused
    attr_reader :background

    def initialize
      @focused = false
      super
    end

    def init_elements
      super
      create_background
    end

    def init_events
      super
      on :resize do
        @background.width = width
        @background.height = height
      end
    end

    def create_background
      @background = SkinSlice9.new
      add(@background)
    end

    def focus
      @focused = true
    end

    def unfocus
      @focused = false
    end

    private :create_background
  end
end
