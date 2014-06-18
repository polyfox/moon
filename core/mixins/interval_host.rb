module IntervalHost

  attr_accessor :intervals

  def add_interval(*args, &block)
    @intervals ||= []
    interval = Interval.new(*args, &block)
    @intervals.push interval
    interval
  end

  def clear_interval(interval)
    @intervals.delete(interval)
  end

  def clear_interval_by_id(id)
    @intervals.delete { |interval| interval.id == id }
  end

  def update_intervals(delta)
    return unless @intervals && !@intervals.empty?

    @intervals.each do |interval|
      interval.update delta
    end
  end

end