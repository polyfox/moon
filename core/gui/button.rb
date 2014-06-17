require 'core/gui/widget'

module Moon
  class Button < Widget

    @@font = Moon::Font.new("resources/fonts/ipaexg00201/ipaexg.ttf", 16)

    def initialize(parent, x, y, text, &block)
      super(parent, x, y, 80, 32)
      @text = text
      @callback = block

      on :click do
        @callback.call
      end
    end

    def update(delta)
      super delta
    end

    def render
      super
      @@font.draw_text(x, y+@@font.size, @text, Moon::Vector4.new(1.0, 0.0, 0.0, 1.0))
    end

  end
end
