#
# core/state/state.rb
#   Base class for all States
class State

  attr_reader :ticks

  @states = []

  def initialize(engine)
    @engine = engine
    @ticks = 0
    init
  end

  # use this instead to initialize a State
  def init
    #
  end

  # Gets called when we close the state
  def terminate
    #
  end

  # Gets called when the state is put on pause and a
  # new state is loaded on top of it
  def pause
    #
  end

  # Gets called when the state resumes
  def resume
    #
  end

  # called when the state is intended to be rendered to screen
  def render
    #
  end

  # Gets called in each game loop iteration
  def update
    @ticks += 1
  end

  def self.debug?
    true
  end

  def self.change state
    last_state = nil
    if !@states.empty?
      @states.last.terminate
      last_state = @states.pop
    end
    @states.push state.new(self)
    puts "[State] change #{last_state.class} >> #{state}" if debug?
  end

  def self.pop
    @states.last.terminate
    last_state = @states.pop

    @states.last.resume if !@states.empty?
    puts "[State] pop #{last_state.class} > #{@states.last.class}" if debug?
    #Engine.stop if @states.empty? # TODO
  end

  def self.push state
    last_state = @states.last
    @states.last.pause if !@states.empty?
    @states.push state.new(self)
    puts "[State] push #{last_state.class} > #{state}" if debug?
  end

end
