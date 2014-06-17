describe Moon::Vector2 do
  it "should create a Vector2 from an Integer" do
    obj = Moon::Vector2[24]
    assert_kind_of Moon::Vector2, obj
    assert_float obj.x, 24.0
    assert_float obj.y, 24.0
  end

  it "should create a Vector2 from a Float" do
    obj = Moon::Vector2[124.8974]
    assert_kind_of Moon::Vector2, obj
    assert_float obj.x, 124.8974
    assert_float obj.y, 124.8974
  end

  it "should create a Vector2 from 2 Integer" do
    obj = Moon::Vector2[12, 24]
    assert_kind_of Moon::Vector2, obj
    assert_float obj.x, 12.0
    assert_float obj.y, 24.0
  end

  it "should create a Vector2 from 2 Floats" do
    obj = Moon::Vector2[7686.3764, 1656.3763]
    assert_kind_of Moon::Vector2, obj
    assert_float obj.x, 7686.3764
    assert_float obj.y, 1656.3763
  end

  it "should create a Vector2 from another Vector2" do
    vec2 = Moon::Vector2.new(5623.0, 7887.0)
    obj = Moon::Vector2[vec2]
    assert_kind_of Moon::Vector2, obj
    assert_float obj.x, 5623.0
    assert_float obj.y, 7887.0
  end
end
