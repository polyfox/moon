class State

  @states = []

  def initialize(engine)
    @engine = engine
  end

  # Gets called when we close the state
  def terminate
  end

  # Gets called when the state is put on pause and a
  # new state is loaded on top of it
  def pause
  end

  # Gets called when the state resumes
  def resume
  end

  # Gets called in each game loop iteration
  def update

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
    @states << state.new(self)
  end

end
