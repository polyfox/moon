class Object
  def try(meth=nil, *args, &block)
    if meth
      self.send(meth, *args, &block)
    else
      yield self
    end
  end
end
