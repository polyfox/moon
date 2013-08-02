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

  def self.change state
    if !@states.empty?
      @states.last.terminate
      @states.pop
    end
    @states.push state.new(self)
  end

  def self.pop
    @states.last.terminate
    @states.pop

    @states.last.resume if !@states.empty?
  end

  def self.push state
    @states.last.pause if !@states.empty?
    @states.push state.new(self)
  end

end
