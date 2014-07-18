class Numeric

  def to_vec2
    Vector2.new self, self
  end

  def to_vec3
    Vector3.new self, self, self
  end

  def masked?(flag)
    if flag == 0
      self == 0
    else
      (self & flag) == flag
    end
  end

end
