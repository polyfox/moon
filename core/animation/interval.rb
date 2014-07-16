class Interval

  attr_reader :id
  attr_reader :time
  attr_reader :duration

  def initialize(duration, &block)
    @time = @duration = duration
    @trigger = block
    @id = SeedRandom.random.base64(16)
  end

  def done?
    @time <= 0
  end

  def update(delta)
    @time -= delta
    if done?
      @trigger.()
      @time = @duration
    end
  end

end