module TaskHost

  ### instance attribute
  attr_accessor :tasks

  ###
  # add_task(duration) { to_execute_on_timeout }
  # @param [Integer] duration
  # @return [Timeout]
  ###
  def add_task(duration, &block)
    @tasks ||= []
    timeout = Timeout.new duration, &block
    @tasks.push timeout
    timeout
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
    return unless @tasks && !@tasks.empty?

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

end
