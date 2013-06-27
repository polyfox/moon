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

  end
end

class State_Test < State
  def initialize(engine)
    super(engine)
    @sprites = []

    15.times do
      @sprites << Sprite.new("obama_sprite.png")
    end
  end

  def update
    @sprites.each {|sprite| sprite.render }
  end
end

Moon.push_state(State_Test)