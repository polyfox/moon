class Easer

  def self.ease(a, b, d)
    a + (b - a)
  end

end

class Easer::Linear < Easer

  def self.ease(a, b, d)
    a + (b - a) * d
  end

end

class Easer::Quad < Easer

  def self.ease(a, b, d)
    a + (b - a) * d * d
  end

end

class Easer::Cubic < Easer

  def self.ease(a, b, d)
    a + (b - a) * d * d * d
  end

end

class Easer::Quart < Easer

  def self.ease(a, b, d)
    a + (b - a) * d * d * d * d
  end

end

class Easer::Quint < Easer

  def self.ease(a, b, d)
    a + (b - a) * d * d * d * d * d
  end

end