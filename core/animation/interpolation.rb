# https://github.com/sole/Tween.js
module Moon
  module Interpolation
    def self.factorial(n)
      a = [1]
      s = 1
      if a[n]
        a[n]
      else
        n.times do |i|
          s *= (n-i)
        end
        a[n] = s
      end
    end

    def self.linear(p0, p1, t)
      (p1 - p0) * t + p0
    end

    def self.bernstein(n, i)
      factorial(n) / factorial(i) / factorial(n - i)
    end

    def self.catmull_rom(p0, p1, p2, p3, t)
      v0 = (p2 - p0) * 0.5
      v1 = (p3 - p1) * 0.5
      t2 = t * t
      t3 = t * t2
      (2 * p1 - 2 * p2 + v0 + v1) * t3 + (-3 * p1 + 3 * p2 - 2 * v0 - v1) * t2 + v0 * t + p1
    end

    Linear = lambda do |v, k|
      m = v.length - 1
      f = m * k
      i = f.floor

      if k < 0
        linear(v[0], v[1], f)
      elsif k > 1
        linear(v[m], v[m - 1], m - f)
      end
      linear(v[i], v[i + 1 > m ? m : i + 1], f - i)
    end

    Bezier = lambda do |v, k|
      b = 0
      n = v.length - 1
      n.times do |i|
        b += ((1 - k) ** (n - i)) * (k ** i) * v[i] * bernstein(n, i)
      end
      b
    end

    CatmullRom = lambda do |v, k|
      m = v.length - 1
      f = m * k
      i = f.floor

      if v[0] == v[m]
        if k < 0
          i = (f = m * (1 + k)).floor
        end
        catmull_rom(v[(i - 1 + m) % m], v[i], v[(i + 1) % m], v[(i + 2) % m], f - i)
      else
        if k < 0
          v[0] - (catmull_rom(v[0], v[0], v[1], v[1], -f) - v[0])
        elsif k > 1
          v[m] - (catmull_rom(v[m], v[m], v[m - 1], v[m - 1], f - m) - v[m])
        else
          catmull_rom(v[i ? i - 1 : 0], v[i], v[m < i + 1 ? m : i + 1], v[m < i + 2 ? m : i + 2], f - i)
        end
      end
    end
  end
end
