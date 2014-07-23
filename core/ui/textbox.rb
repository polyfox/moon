module Moon
  class Textbox < Widget
    attr_accessor :submit_func
    attr_reader :text

    def initialize
      @submit_func = nil
      super
    end

    def init_elements
      super
      @text = Text.new
    end

    def init_events
      super
      on :submit do
        @submit_func.try :call, self
      end

      on :clear do
        @text.string.clear
      end

      on :delete_char do
        @text.string = @text.string.chop
      end
    end
  end
end
