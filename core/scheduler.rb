module Moon
  class Scheduler

    ### instance attribute
    attr_accessor :tasks
    attr_accessor :intervals

    ###
    # @param [String] str
    # @return [Float] duration  in seconds
    ###
    def self.parse_duration(str)
      #   - milliseconds
      # s - seconds
      # m - minutes
      # h - hours
      # d - days
      # w - weeks
      # M - months
      # y - years
      # Now lets be honest here, who would be running this thing for more than
      # a few hours anyway...
      value = 0.0
      str.scan(/(\d+|\d+\.\d+)([smhdwMy])?/).each do |a|
        v = a[0].to_i
        letter = a[1].to_s
        p [v, letter]
        case letter
        when ""
          value += v / 1000.0
        when "s"
          value += v
        when "m"
          value += v * 60.0
        when "h"
          value += v * 60.0 * 60.0
        when "d"
          value += v * 60.0 * 60.0 * 24.0
        when "w"
          value += v * 60.0 * 60.0 * 24.0 * 7.0
        when "M"
          value += v * 60.0 * 60.0 * 24.0 * 7.0 * 30.0
        when "y"
          value += v * 60.0 * 60.0 * 24.0 * 7.0 * 30.0 * 12.0
        end
      end
      value
    end

    def initialize
      @tasks = []
      @intervals = []
    end

    def add_interval(duration, &block)
      duration = self.class.parse_duration(duration) if duration.is_a?(String)
      interval = Interval.new(duration, &block)
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
      return if @intervals.empty?
      @intervals.each do |interval|
        interval.update delta
      end
    end

    ###
    # add_task(duration) { to_execute_on_timeout }
    # @param [Integer] duration
    # @return [Timeout]
    ###
    def add_task(duration, &block)
      duration = self.class.parse_duration(duration) if duration.is_a?(String)
      timeout = Timeout.new(duration, &block)
      @tasks.push timeout
      timeout
    end

    ###
    # Clears all tasks
    ###
    def clear_tasks
      @tasks.clear
    end

    ###
    # @param [Timeout] task
    # @return [Timeout]
    ###
    def remove_task(task)
      @tasks.delete task
    end

    ###
    # @param [Array<Timeout>] tasks
    # @return [Void]
    ###
    def remove_tasks(tasks)
      @tasks -= tasks
    end

    ###
    # @param [Float] delta
    # @return [Void]
    ###
    def update_tasks(delta)
      return if @tasks.empty?
      dead = []
      @tasks.each do |task|
        task.update delta
        dead << task if task.done?
      end
      remove_tasks(dead) unless dead.empty?
    end

    ###
    # Force all tasks to finish.
    # @return [Void]
    ###
    def finish_tasks
      return unless @tasks
      @tasks.each(&:finish)
    end

    def update(delta)
      update_intervals(delta)
      update_tasks(delta)
    end

  end
end
