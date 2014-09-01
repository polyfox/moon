class Hash
  def presence
    empty? ? nil : self
  end

  def exclude(*excluded_keys)
    result = dup
    excluded_keys.each { |key| result.delete(key) }
    result
  end

  def permit(*keys)
    keys.each_with_object({}) { |key, hash| hash[key] = self[key] }
  end

  def symbolize_keys
    each_with_object({}) { |a, hsh| hsh[a[0].to_sym] = a[1] }
  end

  def stringify_keys
    each_with_object({}) { |a, hsh| hsh[a[0].to_s] = a[1] }
  end
end
