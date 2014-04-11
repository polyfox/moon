class Numeric

  def to_vec2
    Vector2.new(self, self)
  end

  def to_vec3
    Vector3.new(self, self, self)
  end

  def masked?(flag)
    (self & flag) == flag
  end

end