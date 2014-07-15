module Moon
  class Scheduler

    ### instance attribute
    attr_accessor :tasks
    attr_accessor :intervals

    def initialize
      @tasks = []
      @intervals = []
    end

    def add_interval(*args, &block)
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
      timeout = Timeout.new duration, &block
      @tasks.push timeout
      timeout
    end

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
