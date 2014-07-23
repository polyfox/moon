module Moon
  module Input
    class Observer
      include Eventable

      def initialize
        init_eventable
      end
    end
  end
end
