module Moon
  module Input

    TRIGGERED_THRESHOLD = 0.020

    module Keyboard

      def self.triggered?(key_id, *args)
        val = (pressed?(key_id, *args) || repeated?(key_id, *args))
        val && val < Moon::Input::TRIGGERED_THRESHOLD
      end

      def self.held?(key_id, *args)
        pressed?(key_id, *args) || repeated?(key_id, *args)
      end

    end
    module Mouse

      BUTTONS = [
        Buttons::BUTTON_1,
        Buttons::BUTTON_2,
        Buttons::BUTTON_3,
        Buttons::BUTTON_4,
        Buttons::BUTTON_5,
        Buttons::BUTTON_6,
        Buttons::BUTTON_7,
        Buttons::BUTTON_8
      ]

      MODS = [0, Input::Keyboard::Keys::MOD_SHIFT, Input::Keyboard::Keys::MOD_CONTROL,
                 Input::Keyboard::Keys::MOD_ALT, Input::Keyboard::Keys::MOD_SUPER]

      def self.in_area?(x, y, width, height)
        self.x.between?(x, x+width) && self.y.between?(y, y+height)
      end

      def self.in_rect?(rect)
        in_area?(rect.x, rect.y, rect.width, rect.height)
      end

      def self.triggered?(key_id, *args)
        val = pressed?(key_id, *args)
        val && val < Moon::Input::TRIGGERED_THRESHOLD
      end

      class << self
        alias :held? :pressed?
      end

    end
  end
end