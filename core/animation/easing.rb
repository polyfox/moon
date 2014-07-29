# https://github.com/sole/Tween.js
module Moon
  module Easing
    Linear = lambda do |k|
      k
    end

    QuadraticIn = lambda do |k|
      k * k
    end

    QuadraticOut = lambda do |k|
      k * (2 - k)
    end

    QuadraticInOut = lambda do |k|
      if (k *= 2) < 1
        0.5 * k * k
      else
        -0.5 * ((k-=1) * (k - 2) - 1)
      end
    end

    CubicIn = lambda do |k|
      k * k * k
    end

    CubicOut = lambda do |k|
      (k-=1) * k * k + 1
    end

    CubicInOut = lambda do |k|
      if (k *= 2) < 1
        0.5 * k * k * k
      else
        0.5 * ((k -= 2) * k * k + 2)
      end
    end

    QuarticIn = lambda do |k|
      k * k * k * k
    end

    QuarticOut = lambda do |k|
      1 - ((k-=1) * k * k * k)
    end

    QuarticInOut = lambda do |k|
      if (k *= 2) < 1
        0.5 * k * k * k * k
      else
        -0.5 * ((k -= 2) * k * k * k - 2)
      end
    end

    QuinticIn = lambda do |k|
      k * k * k * k * k
    end

    QuinticOut = lambda do |k|
      (k-=1) * k * k * k * k + 1
    end

    QuinticInOut = lambda do |k|
      if ((k *= 2) < 1)
        0.5 * k * k * k * k * k
      else
        0.5 * ((k -= 2) * k * k * k * k + 2)
      end
    end

    SinusoidalIn = lambda do |k|
      1 - Math.cos(k * Math::PI / 2)
    end

    SinusoidalOut = lambda do |k|
      Math.sin(k * Math::PI / 2)
    end

    SinusoidalInOut = lambda do |k|
      0.5 * (1 - Math.cos(Math::PI * k))
    end

    ExponentialIn = lambda do |k|
      k == 0 ? 0 : 1024 ** (k - 1)
    end

    ExponentialOut = lambda do |k|
      k == 1 ? 1 : 1 - 2 ** (-10 * k)
    end

    ExponentialInOut = lambda do |k|
      if (k == 0)
        0
      elsif (k == 1)
        1
      elsif ((k *= 2) < 1)
        0.5 * 1024 ** (k - 1)
      else
        0.5 * (-(2 ** (-10 * (k - 1))) + 2)
      end
    end

    CircularIn = lambda do |k|
      1 - Math.sqrt(1 - k * k)
    end

    CircularOut = lambda do |k|
      Math.sqrt(1 - ((k-=1) * k))
    end

    CircularInOut = lambda do |k|
      if ((k *= 2) < 1)
        -0.5 * (Math.sqrt(1 - k * k) - 1)
      else
        0.5 * (Math.sqrt(1 - (k -= 2) * k) + 1)
      end
    end

    ElasticIn = lambda do |k|
      s = nil
      a = 0.1
      p = 0.4
      if (k == 0)
        0
      elsif (k == 1)
        1
      end
      if (!a || a < 1)
        a = 1
        s = p / 4
      else
        s = p * Math.asin(1 / a) / (2 * Math::PI)
      end
      -(a * (2 ** (10 * (k -= 1))) * Math.sin((k - s) * (2 * Math::PI) / p))
    end

    ElasticOut = lambda do |k|
      s = nil
      a = 0.1
      p = 0.4
      if k == 0
        0
      elsif k == 1
        1
      end

      if a == 0 || a < 1
        a = 1
        s = p / 4
      else
        s = p * Math.asin(1 / a) / (2 * Math::PI)
      end
      (a * (2 ** (-10 * k)) * Math.sin((k - s) * (2 * Math::PI) / p) + 1)
    end

    ElasticInOut = lambda do |k|
      s = nil
      a = 0.1
      p = 0.4
      if k == 0
        0
      elsif k == 1
        1
      end

      if a == 0 || a < 1
        a = 1
        s = p / 4
      else
        s = p * Math.asin(1 / a) / (2 * Math::PI)
      end
      if (k *= 2) < 1
        -0.5 * (a * (2 ** (10 * (k -= 1))) * Math.sin((k - s) * (2 * Math::PI) / p))
      else
        a * (2 ** (-10 * (k -= 1))) * Math.sin((k - s) * (2 * Math::PI) / p) * 0.5 + 1
      end
    end

    BackIn = lambda do |k|
      s = 1.70158
      k * k * ((s + 1) * k - s)
    end

    BackOut = lambda do |k|
      s = 1.70158
      (k-=1) * k * ((s + 1) * k + s) + 1;
    end

    BackInOut = lambda do |k|
      s = 1.70158 * 1.525;
      if ((k *= 2) < 1)
        0.5 * (k * k * ((s + 1) * k - s))
      else
        0.5 * ((k -= 2) * k * ((s + 1) * k + s) + 2)
      end
    end

    BounceOut = lambda do |k|
      if k < (1 / 2.75)
        7.5625 * k * k;
      elsif k < (2 / 2.75)
        7.5625 * (k -= (1.5 / 2.75)) * k + 0.75;
      elsif k < (2.5 / 2.75)
        7.5625 * (k -= (2.25 / 2.75)) * k + 0.9375;
      else
        7.5625 * (k -= (2.625 / 2.75)) * k + 0.984375;
      end
    end

    BounceIn = lambda do |k|
      1 - BounceOut.call(1 - k)
    end

    BounceInOut = lambda do |k|
      if k < 0.5
        BounceIn.call(k * 2) * 0.5
      else
        BounceOut.call(k * 2 - 1) * 0.5 + 0.5
      end
    end
  end
end
