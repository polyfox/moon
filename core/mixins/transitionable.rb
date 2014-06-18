module Transitionable

  ### mixins
  include TransitionHost

  ###
  # @param [String] attribute
  #   @example "color"
  #   @example "position.x"
  # @param [Object] value  target value
  # @param [Numeric] duration  in seconds
  ###
  def transition(attribute, value, duration=0.20)
    attribute = attribute.to_s
    rolling = attribute.split(".")
    if rolling.size > 1
      src = rolling.inject(self) { |obj, param| obj.send(param) }
      setter = "#{rolling.pop}="
      add_transition(src, value, duration) do |v|
        rolling.inject(self) { |obj, param| obj.send(param) }.send(setter, v)
      end
    else
      src = send(attribute)
      setter = "#{attribute}="
      add_transition(src, value, duration) { |v| send(setter, v) }
    end
  end

  ###
  # Add a keyed transition
  #   A keyed transition will only have 1 active transition per attribute
  #   However multiple transition can still operate on the attribute if
  #   they where not keyed.
  # @param [String] attribute
  #   @example "color"
  #   @example "position.x"
  # @param [Object] value  target value
  # @param [Numeric] duration  in seconds
  ###
  def key_transition(attribute, value, duration=0.15)
    @key_transition ||= {}
    remove_transition(@key_transition[attribute]) if @key_transition.key?(attribute)
    t = transition(attribute, value, duration)
    t.key = attribute
    @key_transition[attribute] = t
    t
  end

  ###
  # @param [Array<Transitions>] transitions
  # @return [Void]
  ###
  def remove_transitions(transitions)
    if @key_transition
      transitions.each { |t| @key_transition.delete(t.key) if t.key }
    end
    super transitions
  end

end
