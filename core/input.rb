module Moon
  module Input

    STRING_TO_KEY = {
      "space" => SPACE,
      "apostrophe" => APOSTROPHE,
      "comma" => COMMA,
      "minus" => MINUS,
      "period" => PERIOD,
      "slash" => SLASH,
      "n0" => N0,
      "n1" => N1,
      "n2" => N2,
      "n3" => N3,
      "n4" => N4,
      "n5" => N5,
      "n6" => N6,
      "n7" => N7,
      "n8" => N8,
      "n9" => N9,
      "semicolon" => SEMICOLON,
      "equal" => EQUAL,
      "plus" => EQUAL,
      "a" => A,
      "b" => B,
      "c" => C,
      "d" => D,
      "e" => E,
      "f" => F,
      "g" => G,
      "h" => H,
      "i" => I,
      "j" => J,
      "k" => K,
      "l" => L,
      "m" => M,
      "n" => N,
      "o" => O,
      "p" => P,
      "q" => Q,
      "r" => R,
      "s" => S,
      "t" => T,
      "u" => U,
      "v" => V,
      "w" => W,
      "x" => X,
      "y" => Y,
      "z" => Z,
      "left_bracket" => LEFT_BRACKET,
      "backslash" => BACKSLASH,
      "right_bracket" => RIGHT_BRACKET,
      "grave_accent" => GRAVE_ACCENT,
      "world_1" => WORLD_1,
      "world_2" => WORLD_2,
      "escape" => ESCAPE,
      "enter" => ENTER,
      "tab" => TAB,
      "backspace" => BACKSPACE,
      "insert" => INSERT,
      "delete" => DELETE,
      "right" => RIGHT,
      "left" => LEFT,
      "down" => DOWN,
      "up" => UP,
      "page_up" => PAGE_UP,
      "page_down" => PAGE_DOWN,
      "home" => HOME,
      "end" => const_get(:END),
      "caps_lock" => CAPS_LOCK,
      "scroll_lock" => SCROLL_LOCK,
      "num_lock" => NUM_LOCK,
      "print_screen" => PRINT_SCREEN,
      "pause" => PAUSE,
      "f1" => F1,
      "f2" => F2,
      "f3" => F3,
      "f4" => F4,
      "f5" => F5,
      "f6" => F6,
      "f7" => F7,
      "f8" => F8,
      "f9" => F9,
      "f10" => F10,
      "f11" => F11,
      "f12" => F12,
      "f13" => F13,
      "f14" => F14,
      "f15" => F15,
      "f16" => F16,
      "f17" => F17,
      "f18" => F18,
      "f19" => F19,
      "f20" => F20,
      "f21" => F21,
      "f22" => F22,
      "f23" => F23,
      "f24" => F24,
      "f25" => F25,
      "kp_0" => KP_0,
      "kp_1" => KP_1,
      "kp_2" => KP_2,
      "kp_3" => KP_3,
      "kp_4" => KP_4,
      "kp_5" => KP_5,
      "kp_6" => KP_6,
      "kp_7" => KP_7,
      "kp_8" => KP_8,
      "kp_9" => KP_9,
      "kp_decimal" => KP_DECIMAL,
      "kp_divide" => KP_DIVIDE,
      "kp_multiply" => KP_MULTIPLY,
      "kp_subtract" => KP_SUBTRACT,
      "kp_add" => KP_ADD,
      "kp_enter" => KP_ENTER,
      "kp_equal" => KP_EQUAL,
      "left_shift" => LEFT_SHIFT,
      "left_control" => LEFT_CONTROL,
      "left_alt" => LEFT_ALT,
      "left_super" => LEFT_SUPER,
      "right_shift" => RIGHT_SHIFT,
      "right_control" => RIGHT_CONTROL,
      "right_alt" => RIGHT_ALT,
      "right_super" => RIGHT_SUPER,
      "menu" => MENU,
      "mouse_button_1" => MOUSE_BUTTON_1,
      "mouse_button_2" => MOUSE_BUTTON_2,
      "mouse_button_3" => MOUSE_BUTTON_3,
      "mouse_button_4" => MOUSE_BUTTON_4,
      "mouse_button_5" => MOUSE_BUTTON_5,
      "mouse_button_6" => MOUSE_BUTTON_6,
      "mouse_button_7" => MOUSE_BUTTON_7,
      "mouse_button_8" => MOUSE_BUTTON_8,
      "mouse_left" => MOUSE_LEFT,
      "mouse_right" => MOUSE_RIGHT,
      "mouse_middle" => MOUSE_MIDDLE,
    }

    def self.on_key(key, scancode, action, mods)
      State.current.input.trigger KeyboardEvent.new(key, action, mods)
    end

    def self.on_button(button, action, mods)
      State.current.input.trigger MouseEvent.new(button, action, mods, Mouse.position)
    end

    def self.on_type(char)
      State.current.input.trigger KeyboardTypeEvent.new(char)
    end

    def self.convert_key(key)
      (key.is_a?(Symbol)||key.is_a?(String)) ? STRING_TO_KEY.fetch(key.to_s) : key
    end

    class Observer
      def initialize
        clear
      end

      ###
      # Adds a new event listener.
      # @param [Symbol] keys The keys to listen for..
      # @param [Proc] block The block we want to execute when we catch the type.
      ###
      def on(action, *keys, &block)
        keys = keys.flatten.map! { |k| Input.convert_key(k) }
        listener = { block: block }
        listener[:keys] = keys unless keys.empty?
        @event_listeners[action].push(listener)
      end

      def typing(&block)
        on(:type, &block)
      end

      def alias_event(newname, key)
        (@aliases[key] ||= []).push(newname)
      end

      def trigger_event(name, event)
        @event_listeners[name].each do |listener|
          if listener.key?(:keys)
            listener[:block].call(event) if listener[:keys].include?(event.key)
          else
            listener[:block].call(event)
          end
        end
      end

      def trigger_aliases(name, event)
        @aliases[name].try(:each) do |aliasname|
          trigger_event(aliasname, event)
        end
      end

      def trigger_any(event)
        trigger_event(:any, event)
      end

      ###
      # @param [Event] event
      ###
      def trigger(event)
        trigger_any(event)
        trigger_event(event.action, event)
        trigger_aliases(event.action, event)
      end

      def clear
        @event_listeners = {
          any: [], press: [], release: [], repeat: [], type: []
        }
        @aliases = {}
      end
      private :convert_key
      private :trigger_event
      private :trigger_aliases
    end

    module Mouse
      BUTTONS = [ MOUSE_BUTTON_1,
                  MOUSE_BUTTON_2,
                  MOUSE_BUTTON_3,
                  MOUSE_BUTTON_4,
                  MOUSE_BUTTON_5,
                  MOUSE_BUTTON_6,
                  MOUSE_BUTTON_7,
                  MOUSE_BUTTON_8 ]

      MODS = [0, MOD_SHIFT, MOD_CONTROL, MOD_ALT, MOD_SUPER]

      def self.in_area?(x, y, width, height)
        self.x.between?(x, x+width) && self.y.between?(y, y+height)
      end

      def self.in_rect?(rect)
        in_area?(rect.x, rect.y, rect.width, rect.height)
      end
    end
  end
end
#------------------------------------------------------------------------------#
# Observer, watches the input for us, and distributes events to
# subscribed objects.
#
# Warning, do not include it
#
#----[ Event based input handling ]--------------------------------------------#
#
# When a button or a key gets pressed, a "press(ed)" event triggers exactly once (one frame). On release, a "release(d)" event gets triggered for one frame.
# (same as mousedown, mouseup).
#
# When we detect a mousedown, followed by a mouseup, we've detected a "click", which triggers exactly once. A "dblclick"/"double click" is when one click
# event follows another, in a gap of less than 500ms.
#
# mouseover - mouse is in the area of the object.
# mouseout - mouse is no longer in the area of the object
# mousemove - the mouse has moved
# mousewheel - the wheel has moved
#
# if the mouse is pressed down (mousedown) and we get a mousemove event, that means we have a "drag" event.
# upon releasing the mouse (mouseup), we have a "drop" event.
#
# "current click count" - consecutive clicks in a certain time frame
#
# http://www.w3.org/TR/DOM-Level-3-Events/#event-type-click
#
# For click detection, we need to match the mouseup and mousedown events, because for example if we mousedown on
# an object, then move the mouse away from the object and release the mouse, then click down and pull the mouse
# over the object, and release it, a click should not be triggered, because essentially, we just made 2 different
# clicks.
# To solve this, let's assign an ID to each mouse press/release pair, and store the last mousedown ID. When we
# get a mouseup, we check if the ID matches the mousedown ID. if it does, it's the same click, and we trigger
# a click event, else we just trash it.
#
#----[ HTML5 ]-----------------------------------------------------------------#
#
# drag: fired frequently on a dragged element (which must define the *draggable* attribute.)
# dragstart: fired when the mouse is held down on an element and the movement starts.
# dragend: fired when the element is released.
# dragenter: fired when an element enters the displayed area of another one.
# dragleave: fired when an element exits the displayed area, as the inverse of dragenter.
# dragover: fired frequently when an element is over another.
# drop: fired when an element is released over another.
#------------------------------------------------------------------------------#
