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

class State_Test < State

  def initialize(engine)
    super(engine)
    @sprites = []

    @sound = Sound.new("startup.wav", "wav")
    @sound.play
    @s = Spritesheet.new("hyptosis_tile-art-batch-1.png", 32, 32)
    # 6 FPS at 15000 items
    # 55 FPS at 1500 items
    #1500.times do
      #sprite = Sprite.new("obama_sprite.png")
      #sprite.x = rand(640)
      #sprite.y = rand(480)
      #@sprites << sprite
    #end
  end

  def update
    for i in 0..1800 # runs efficiently at 60FPS, 18000 runs at 20FPS
      @s.render(i, i, i)
    end
    #@sprites.each {|sprite| sprite.render }
  end

end

State.push(State_Test)
