class Interval
  ### instance attributes
  attr_reader :id
  attr_reader :time
  attr_reader :duration

  ###
  # @param [Float] duration
  ###
  def initialize(duration, &block)
    @time = @duration = duration
    @trigger = block
    @id = SeedRandom.random.base64(16)
  end

  ###
  # Intervals are never done.
  # @return [Boolean]
  ###
  def done?
    false
  end

  ###
  # @param [Float] delta
  # @return [Void]
  ###
  def update(delta)
    @time -= delta
    if @time <= 0
      @trigger.()
      @time = @duration
    end
  end

  ###
  # Force end the interval
  # Intervals do nothing on #finish
  ###
  def finish
    #
  end
end
