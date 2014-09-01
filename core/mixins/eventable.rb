module Moon
  module Eventable
    def init_eventable
      clear_events
    end

    def clear_events
      @event_listeners = {}
      @aliases = {}
    end

    ###
    # @return [Boolean]
    ###
    def allow_event?(event)
      true
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
      (@event_listeners[action] ||= []).push(listener)
      listener
    end

    def typing(&block)
      on(:typing, &block)
    end

    def alias_event(newname, key)
      (@aliases[key] ||= []).push(newname)
    end

    def trigger_event(name, event)
      @event_listeners[name].try(:each) do |listener|
        if listener.key?(:keys)
          listener[:block].call(event, self) if listener[:keys].include?(event.key)
        else
          listener[:block].call(event, self)
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
      event = Event.new(event) unless event.is_a?(Event)

      return unless allow_event?(event)

      trigger_any(event)
      trigger_event(event.type, event)
      trigger_aliases(event.type, event)
    end

    private :trigger_event
    private :trigger_aliases
    private :trigger_any
  end
end
