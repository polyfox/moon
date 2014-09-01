class Object
  def presence
    self || nil
  end

  def to_link_node
    LinkNode.new(self)
  end

  def try(meth=nil, *args, &block)
    if meth
      self.send(meth, *args, &block)
    else
      yield self
    end
  end
end
