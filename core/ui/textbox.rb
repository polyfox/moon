module Moon
  class Textbox < Widget
    attr_reader :text

    def init_elements
      super
      @text = Text.new
      add(@text)
    end

    def init_events
      super
      on :typing do |e|
        @text.string += e.char
      end

      on :clear do
        @text.string.clear
      end

      on :press, :backspace do
        @text.string = @text.string.chop
      end

      on :resize do
        @text.position.y = (height - @text.line_height) / 2
      end
    end
  end
end
