class Timeout

  ### class variables
  @@id = 0

  ### instance attributes
  attr_reader :id
  attr_reader :time
  attr_reader :duration
  attr_reader :active

  ###
  # @param [Float] duration
  ###
  def initialize(duration, &block)
    @id = @@id += 1
    @time = @duration = duration
    @trigger = block
    @active = true
  end

  ###
  # @return [Boolean]
  ###
  def done?
    @time <= 0
  end

  ###
  # @param [Float] delta
  # @return [Void]
  ###
  def update(delta)
    return unless @active
    @time -= delta
    if done?
      @trigger.()
      @active = false
    end
  end

  ###
  # @return [Void]
  ###
  def finish
    @time = 0.0
    @trigger.()
  end

end
