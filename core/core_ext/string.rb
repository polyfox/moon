class String

  def demodulize
    path = self.to_s
    if i = path.rindex('::')
      path[(i+2)..-1]
    else
      path
    end
  end

end