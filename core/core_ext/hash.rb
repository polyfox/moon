class Hash

  def symbolize_keys
    each_with_object({}) { |a, hsh| hsh[a[0].to_sym] = a[1] }
  end

  def stringify_keys
    each_with_object({}) { |a, hsh| hsh[a[0].to_s] = a[1] }
  end

end