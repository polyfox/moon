describe Moon::Interpolation::Linear do
  it "should interpolate @ 0%" do
    Moon::Interpolation::Linear.call([0, 12, 23, 48], 0.0)
  end
  it "should interpolate @ 25%" do
    Moon::Interpolation::Linear.call([0, 12, 23, 48], 0.25)
  end
  it "should interpolate @ 50%" do
    Moon::Interpolation::Linear.call([0, 12, 23, 48], 0.5)
  end
  it "should interpolate @ 75%" do
    Moon::Interpolation::Linear.call([0, 12, 23, 48], 0.75)
  end
  it "should interpolate @ 100%" do
    Moon::Interpolation::Linear.call([0, 12, 23, 48], 1.0)
  end
end

describe Moon::Interpolation::Bezier do
  it "should interpolate @ 0%" do
    Moon::Interpolation::Bezier.call([0, 12, 23, 48], 0.0)
  end
  it "should interpolate @ 25%" do
    Moon::Interpolation::Bezier.call([0, 12, 23, 48], 0.25)
  end
  it "should interpolate @ 50%" do
    Moon::Interpolation::Bezier.call([0, 12, 23, 48], 0.5)
  end
  it "should interpolate @ 75%" do
    Moon::Interpolation::Bezier.call([0, 12, 23, 48], 0.75)
  end
  it "should interpolate @ 100%" do
    Moon::Interpolation::Bezier.call([0, 12, 23, 48], 1.0)
  end
end

describe Moon::Interpolation::CatmullRom do
  it "should interpolate @ 0%" do
    Moon::Interpolation::CatmullRom.call([0, 12, 23, 48], 0.0)
  end
  it "should interpolate @ 25%" do
    Moon::Interpolation::CatmullRom.call([0, 12, 23, 48], 0.25)
  end
  it "should interpolate @ 50%" do
    Moon::Interpolation::CatmullRom.call([0, 12, 23, 48], 0.5)
  end
  it "should interpolate @ 75%" do
    Moon::Interpolation::CatmullRom.call([0, 12, 23, 48], 0.75)
  end
  it "should interpolate @ 100%" do
    Moon::Interpolation::CatmullRom.call([0, 12, 23, 48], 1.0)
  end
end
