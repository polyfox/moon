module Moon
  module Input

    def self.on_key(key, scancode, action, mods)
    end

    def self.on_button(button, action, mods)
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