class Interval

  @@id = 0

  attr_reader :id
  attr_reader :time
  attr_reader :duration

  def initialize(duration, &block)
    @time = @duration = duration
    @trigger = block
    @id = @@id += 1
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