# https://github.com/sole/Tween.js
module Moon
  module Interpolation
    LinearUtil = lambda do |p0, p1, t|
      (p1 - p0) * t + p0
    end

    BernsteinUtil = lambda do |n, i|
      fc = UtilFactorial
      fc.call(n) / fc.call(i) / fc.call(n - i)
    end

    FactorialUtil = lambda do |n|
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

    CatmullRomUtil = lambda do |p0, p1, p2, p3, t|
      v0 = (p2 - p0) * 0.5
      v1 = (p3 - p1) * 0.5
      t2 = t * t, t3 = t * t2
      (2 * p1 - 2 * p2 + v0 + v1) * t3 + (-3 * p1 + 3 * p2 - 2 * v0 - v1) * t2 + v0 * t + p1
    end

    Linear = lambda do |v, k|
      m = v.length - 1
      f = m * k
      i = f.floor
      fn = LinearUtil

      if k < 0
        fn.call(v[0], v[1], f)
      elsif k > 1
        fn.call(v[m], v[m - 1], m - f)
      end
      fn.call(v[i], v[i + 1 > m ? m : i + 1], f - i)
    end

    Bezier = lambda do |v, k|
      b = 0
      n = v.length - 1
      bn = BernsteinUtil

      n.times do |i|
        b += ((1 - k) ** (n - i)) * (k ** i) * v[i] * bn(n, i)
      end

      b
    end

    CatmullRom = lambda do |v, k|
      m = v.length - 1
      f = m * k
      i = f.floor
      fn = CatmullRomUtil

      if v[0] == v[m]
        if k < 0
          i = (f = m * (1 + k)).floor
        end
        fn.call(v[(i - 1 + m) % m], v[i], v[(i + 1) % m], v[(i + 2) % m], f - i)
      else
        if k < 0
          v[0] - (fn.call(v[0], v[0], v[1], v[1], -f) - v[0])
        elsif k > 1
          v[m] - (fn.call(v[m], v[m], v[m - 1], v[m - 1], f - m) - v[m])
        else
          fn.call(v[i ? i - 1 : 0], v[i], v[m < i + 1 ? m : i + 1], v[m < i + 2 ? m : i + 2], f - i)
        end
      end
    end
  end
end
