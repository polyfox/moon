module Moon
  extend self

  @states = []

  def change_state state
    if !@states.empty?
      @states.last.terminate
      @states.pop
    end
    @states.push state.new(self)
  end

  def pop_state
    @states.last.terminate
    @states.pop

    @states.last.resume if !@states.empty?
  end

  def push_state state
    @states.last.pause if !@states.empty?
    @states << state.new(self)
  end
end