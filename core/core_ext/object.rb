class Object

  def try(meth=nil, *args, &block)
    if block_given?
      yield self
    else
      self.send(meth, *args, &block)
    end
  end

end
