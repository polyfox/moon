class Module

  alias :__const_get__ :const_get

  def const_get(path)
    path.split("::").inject(Object) { |klass, name| klass.__const_get__(name) }
  end

end