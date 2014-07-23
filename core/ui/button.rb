module Moon
  class Button < Widget
    attr_reader :text

    def initialize
      super
      @text = Text.new
    end
  end
end
