module Moon
  class Transition
    ### class variables
    @@id = 0

    ### instance attributes
    attr_accessor :key
    attr_accessor :easer
    attr_accessor :src
    attr_accessor :dest
    attr_reader :id
    attr_reader :time
    attr_reader :duration

    ###
    # initialize(src, dest, duration) { |value| transition_callback }
    # @param [Object] src
    # @param [Object] dest
    # @param [Float] duration  in seconds
    # @return [Void]
    ###
    def initialize(src, dest, duration, easer=Easing::Linear, &block)
      @id = @@id += 1
      @key = nil
      @src = src
      @dest = dest
      @time = 0.0
      @duration = duration
      @easer = easer
      @callback = block
    end

    ###
    # @return [Boolean]
    ###
    def done?
      @time >= @duration
    end

    ###
    # @return [Void]
    ###
    def refresh
      @callback.call(@src + (@dest - @src) * @easer.call(@time / @duration))
    end

    ###
    # @param [Float] delta
    # @return [Void]
    ###
    def update(delta)
      return if done?
      @time += delta
      @time = @duration if @time > @duration
      refresh
    end

    ###
    # @return [Void]
    ###
    def finish
      @time = @duration
      refresh
    end
  end
end
