module Moon
  class Label < Widget
    attr_reader :text

    def initialize
      super
      @text = Text.new
    end
  end
end
