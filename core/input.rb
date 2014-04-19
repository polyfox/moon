module Moon
  module Input

    def self.on_key key, scancode, action, mods
      state = State.states.last # delagator shim
      state.input.trigger Event.new(key, action, mods)
    end

    def self.on_button button, action, mods
      state = State.states.last # delagator shim
      state.input.trigger Event.new(button, action, mods)
    end

    class Observer
      def initialize
        @event_listeners = {any: [], press: [], release: [], repeat: []}
      end

      ###
      # Adds a new event listener.
      # @param [Symbol] keys The keys to listen for..
      # @param [Proc] block The block we want to execute when we catch the type.
      ###
      def on action, *keys, &block
        @event_listeners[action].push(keys: keys, block: block)
      end

      ###
      # @param [Event] event
      ###
      def trigger event
        @event_listeners[:any].each do |listener|
          listener[:block].call(event)
        end

        return unless @event_listeners[event.type]

        @event_listeners[event.type].each do |listener|
          listener[:block].call(event) if listener[:keys].include? event.key
        end
      end

      def clear
        @event_listeners = {any: [], press: [], release: [], repeat: []}
      end
    end

    module Mouse

      BUTTONS = [ BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5,
                  BUTTON_6, BUTTON_7, BUTTON_8 ]

      MODS = [0, Input::Keyboard::MOD_SHIFT, Input::Keyboard::MOD_CONTROL,
                 Input::Keyboard::MOD_ALT, Input::Keyboard::MOD_SUPER]

      def self.in_area?(x, y, width, height)
        self.x.between?(x, x+width) && self.y.between?(y, y+height)
      end

      def self.in_rect?(rect)
        in_area?(rect.x, rect.y, rect.width, rect.height)
      end

      def self.pos
        Vector2.new x, y
      end

    end
  end
end