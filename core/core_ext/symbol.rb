class Symbol

  #def to_proc
  #  -> (obj, *args, &block) { obj.send(self, *args, &block) }
  #end unless method_defined? :to_proc

  # oh look, &:symbol solution
  def call(obj, *args, &block)
    obj.send(self, *args, &block)
  end

end