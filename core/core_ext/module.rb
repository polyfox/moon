class Module

  alias :__const_get__ :const_get

  ###
  # const_get resolves namespaces and top level constants!
  # @param [String] path
  ###
  def const_get(path)
    top = self
    paths = path.to_s.split("::")
    if path.to_s.start_with?("::")
      top = Object
      paths.shift
    end
    paths.inject(top) { |klass, name| klass.__const_get__(name) }
  end

end
