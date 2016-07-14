module Moon
  # Input is used for event-based input processing. It supports a keyboard,
  # mouse and gamepads.
  class Input
    # [Hash<Integer, Symbol>] Maps glfw key states to symbols
    STATE_MAP = {
      GLFW::PRESS => :press,
      GLFW::RELEASE => :release,
      GLFW::REPEAT => :repeat,
    }

    # [Hash<Integer, Symbol>] Maps glfw key codes to Symbols
    KEY_MAP = {
      GLFW::KEY_SPACE => :space,
      GLFW::KEY_APOSTROPHE => :apostrophe,
      GLFW::KEY_COMMA => :comma,
      GLFW::KEY_MINUS => :minus,
      GLFW::KEY_PERIOD => :period,
      GLFW::KEY_SLASH => :slash,
      GLFW::KEY_0 => :n0,
      GLFW::KEY_1 => :n1,
      GLFW::KEY_2 => :n2,
      GLFW::KEY_3 => :n3,
      GLFW::KEY_4 => :n4,
      GLFW::KEY_5 => :n5,
      GLFW::KEY_6 => :n6,
      GLFW::KEY_7 => :n7,
      GLFW::KEY_8 => :n8,
      GLFW::KEY_9 => :n9,
      GLFW::KEY_SEMICOLON => :semicolon,
      GLFW::KEY_EQUAL => :equal,
      GLFW::KEY_A => :a,
      GLFW::KEY_B => :b,
      GLFW::KEY_C => :c,
      GLFW::KEY_D => :d,
      GLFW::KEY_E => :e,
      GLFW::KEY_F => :f,
      GLFW::KEY_G => :g,
      GLFW::KEY_H => :h,
      GLFW::KEY_I => :i,
      GLFW::KEY_J => :j,
      GLFW::KEY_K => :k,
      GLFW::KEY_L => :l,
      GLFW::KEY_M => :m,
      GLFW::KEY_N => :n,
      GLFW::KEY_O => :o,
      GLFW::KEY_P => :p,
      GLFW::KEY_Q => :q,
      GLFW::KEY_R => :r,
      GLFW::KEY_S => :s,
      GLFW::KEY_T => :t,
      GLFW::KEY_U => :u,
      GLFW::KEY_V => :v,
      GLFW::KEY_W => :w,
      GLFW::KEY_X => :x,
      GLFW::KEY_Y => :y,
      GLFW::KEY_Z => :z,
      GLFW::KEY_LEFT_BRACKET => :left_bracket,
      GLFW::KEY_BACKSLASH => :backslash,
      GLFW::KEY_RIGHT_BRACKET => :right_bracket,
      GLFW::KEY_GRAVE_ACCENT => :grave_accent,
      GLFW::KEY_WORLD_1 => :world_1,
      GLFW::KEY_WORLD_2 => :world_2,
      GLFW::KEY_ESCAPE => :escape,
      GLFW::KEY_ENTER => :enter,
      GLFW::KEY_TAB => :tab,
      GLFW::KEY_BACKSPACE => :backspace,
      GLFW::KEY_INSERT => :insert,
      GLFW::KEY_DELETE => :delete,
      GLFW::KEY_RIGHT => :right,
      GLFW::KEY_LEFT => :left,
      GLFW::KEY_DOWN => :down,
      GLFW::KEY_UP => :up,
      GLFW::KEY_PAGE_UP => :page_up,
      GLFW::KEY_PAGE_DOWN => :page_down,
      GLFW::KEY_HOME => :home,
      GLFW::KEY_END => :end,
      GLFW::KEY_CAPS_LOCK => :caps_lock,
      GLFW::KEY_SCROLL_LOCK => :scroll_lock,
      GLFW::KEY_NUM_LOCK => :num_lock,
      GLFW::KEY_PRINT_SCREEN => :print_screen,
      GLFW::KEY_PAUSE => :pause,
      GLFW::KEY_F1 => :f1,
      GLFW::KEY_F2 => :f2,
      GLFW::KEY_F3 => :f3,
      GLFW::KEY_F4 => :f4,
      GLFW::KEY_F5 => :f5,
      GLFW::KEY_F6 => :f6,
      GLFW::KEY_F7 => :f7,
      GLFW::KEY_F8 => :f8,
      GLFW::KEY_F9 => :f9,
      GLFW::KEY_F10 => :f10,
      GLFW::KEY_F11 => :f11,
      GLFW::KEY_F12 => :f12,
      GLFW::KEY_F13 => :f13,
      GLFW::KEY_F14 => :f14,
      GLFW::KEY_F15 => :f15,
      GLFW::KEY_F16 => :f16,
      GLFW::KEY_F17 => :f17,
      GLFW::KEY_F18 => :f18,
      GLFW::KEY_F19 => :f19,
      GLFW::KEY_F20 => :f20,
      GLFW::KEY_F21 => :f21,
      GLFW::KEY_F22 => :f22,
      GLFW::KEY_F23 => :f23,
      GLFW::KEY_F24 => :f24,
      GLFW::KEY_F25 => :f25,
      GLFW::KEY_KP_0 => :kp_0,
      GLFW::KEY_KP_1 => :kp_1,
      GLFW::KEY_KP_2 => :kp_2,
      GLFW::KEY_KP_3 => :kp_3,
      GLFW::KEY_KP_4 => :kp_4,
      GLFW::KEY_KP_5 => :kp_5,
      GLFW::KEY_KP_6 => :kp_6,
      GLFW::KEY_KP_7 => :kp_7,
      GLFW::KEY_KP_8 => :kp_8,
      GLFW::KEY_KP_9 => :kp_9,
      GLFW::KEY_KP_DECIMAL => :kp_decimal,
      GLFW::KEY_KP_DIVIDE => :kp_divide,
      GLFW::KEY_KP_MULTIPLY => :kp_multiply,
      GLFW::KEY_KP_SUBTRACT => :kp_subtract,
      GLFW::KEY_KP_ADD => :kp_add,
      GLFW::KEY_KP_ENTER => :kp_enter,
      GLFW::KEY_KP_EQUAL => :kp_equal,
      GLFW::KEY_LEFT_SHIFT => :left_shift,
      GLFW::KEY_LEFT_CONTROL => :left_control,
      GLFW::KEY_LEFT_ALT => :left_alt,
      GLFW::KEY_LEFT_SUPER => :left_super,
      GLFW::KEY_RIGHT_SHIFT => :right_shift,
      GLFW::KEY_RIGHT_CONTROL => :right_control,
      GLFW::KEY_RIGHT_ALT => :right_alt,
      GLFW::KEY_RIGHT_SUPER => :right_super,
      GLFW::KEY_MENU => :menu,
      GLFW::MOUSE_BUTTON_4 => :mouse_button_4,
      GLFW::MOUSE_BUTTON_5 => :mouse_button_5,
      GLFW::MOUSE_BUTTON_6 => :mouse_button_6,
      GLFW::MOUSE_BUTTON_7 => :mouse_button_7,
      GLFW::MOUSE_BUTTON_8 => :mouse_button_8,
      GLFW::MOUSE_BUTTON_LEFT => :mouse_left,
      GLFW::MOUSE_BUTTON_RIGHT => :mouse_right,
      GLFW::MOUSE_BUTTON_MIDDLE => :mouse_middle,
    }

    MOD_SHIFT = GLFW::MOD_SHIFT
    MOD_CONTROL = GLFW::MOD_CONTROL
    MOD_ALT = GLFW::MOD_ALT
    MOD_SUPER = GLFW::MOD_SUPER

    # @return [GLFW::Window]
    attr_reader :window
    # @return [Moon::Input::Mouse]
    attr_reader :mouse
    # @return [Moon::ContextLogger, ILogger] any logger like interface
    attr_accessor :logger
    #attr_reader :keyboard

    # @param [GLFW::Window] window
    def initialize(window)
      @logger = Moon::ContextLogger.new STDERR, 'Input'
      @window = window
      init
    end

    # Initializes handlers and callbacks
    #
    # @api private
    def init
      initialize_mouse
      register_callbacks
    end

    private def initialize_mouse
      @logger.debug 'Initializing Mouse Handler'
      @mouse = Mouse.new @window
      @logger.debug 'Mouse Handler Initialized'
    end

    private def register_callbacks
      @logger.debug 'Registering Callbacks'
      @window.set_key_callback do |_, key_id, scancode, action, mods|
        on_key KEY_MAP.fetch(key_id), scancode, STATE_MAP.fetch(action), mods unless key_id == -1
      end

      @window.set_mouse_button_callback do |_, button_id, action, mods|
        on_button KEY_MAP.fetch(button_id), STATE_MAP.fetch(action), mods
      end

      @window.set_char_callback do |_, char|
        on_type char.chr
      end

      @window.set_cursor_pos_callback do |_, x, y|
        on_mousemove x, y
      end
      @logger.debug 'Callbacks Registered'
    end

    private def unregister_callbacks
      @logger.debug 'Unregistering Callbacks'
      @window.set_key_callback
      @window.set_mouse_button_callback
      @window.set_char_callback
      @window.set_cursor_pos_callback
      @logger.debug 'Callbacks Unregistered'
    end

    # Removes all handlers to the underlying window
    #
    # @api private
    def shutdown
      @logger.debug 'Shutting down'
      unregister_callbacks
      @mouse = nil
      @logger.debug 'Shutdown'
    end

    # Called on every keypress.
    #
    # @param [Symbol] key symbol id representation of the key
    # @param [Integer] scancode platform specific key scan code
    # @param [Symbol] action new state of the key (:press, :repeat, :release)
    # @param [Integer] mods bits representing the modifier keys (match against
    # with bitmasks)
    def on_key(key, scancode, action, mods)
    end

    # Called on every (mouse) button press.
    # @param [Symbol] button symbol id representation of the button
    # @param [Symbol] action new state of the key (:press, :repeat, :release)
    # @param [Integer] mods bits representing the modifier keys (match against
    #   with bitmasks)
    def on_button(button, action, mods)
    end

    # Called on keypress when the key is a printable character. Useful when
    # reading for GUI/textbox inputs.
    # @param [String] char Character representation of the key
    def on_type(char)
    end

    # Called while the mouse is moving.
    # @param [Integer] x new x coordinate
    # @param [Integer] y new y coordinate
    def on_mousemove(x, y)
    end
  end

  class Input::Mouse
    attr_reader :window

    # @param [GLFW::Window] window
    def initialize(window)
      @window = window
    end

    # @return [Float] the x coordinate of the mouse
    def x
      @window.cursor_pos[0]
    end

    # @return [Float] the y coordinate of the mouse
    def y
      @window.cursor_pos[1]
    end

    # @return [Moon::Vector2] the coordinates of the mouse
    def position
      Vector2[@window.cursor_pos]
    end
  end
end
