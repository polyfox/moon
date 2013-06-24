class State
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
    p "Updated!"
    #Moon.push_state(Staty)
  end
end

class Staty < State
  def updated
    p "hipster state!"
  end
end

Moon.push_state(State)