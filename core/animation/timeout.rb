class Timeout
  ### instance attributes
  attr_reader :id
  attr_reader :time
  attr_reader :duration
  attr_reader :active

  ###
  # @param [Float] duration
  ###
  def initialize(duration, &block)
    @id = Random.random.base64(16)
    @time = @duration = duration
    @trigger = block
    @active = true
  end

  ###
  # Has this timeout reached its end?
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
  # Forces the Timeout to finish prematurely
  # @return [Void]
  ###
  def finish
    @time = 0.0
    @trigger.()
  end
end
