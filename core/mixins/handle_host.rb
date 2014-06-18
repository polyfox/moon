module HandleHost

  def init_handles
    @handles ||= {}
  end

  def handle(event, &block)
    init_handles
    @handles[event] = block
  end

  def trigger_handle(key, *args, &block)
    @handles && @handles[key].try(:call, *args, &block)
  end

end
