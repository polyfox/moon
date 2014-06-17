describe Moon::Vector3 do
  it "should create a Vector3 from an Integer" do
    obj = Moon::Vector3[24]
    assert_kind_of Moon::Vector3, obj
    assert_float obj.x, 24.0
    assert_float obj.y, 24.0
    assert_float obj.z, 24.0
  end

  it "should create a Vector3 from a Float" do
    obj = Moon::Vector3[124.8974]
    assert_kind_of Moon::Vector3, obj
    assert_float obj.x, 124.8974
    assert_float obj.y, 124.8974
    assert_float obj.z, 124.8974
  end

  it "should create a Vector3 from 3 Integer" do
    obj = Moon::Vector3[67, 45, 82]
    assert_kind_of Moon::Vector3, obj
    assert_float obj.x, 67.0
    assert_float obj.y, 45.0
    assert_float obj.z, 82.0
  end

  it "should create a Vector3 from 3 Floats" do
    obj = Moon::Vector3[7686.37649, 1656.3763, 8767.8679]
    assert_kind_of Moon::Vector3, obj
    assert_float obj.x, 7686.37649
    assert_float obj.y, 1656.3763
    assert_float obj.z, 8767.8679
  end

  it "should create a Vector3 from another Vector3" do
    vec2 = Moon::Vector3.new(5623.0, 7887.0, 5653.873)
    obj = Moon::Vector3[vec2]
    assert_kind_of Moon::Vector3, obj
    assert_float obj.x, 5623.0
    assert_float obj.y, 7887.0
    assert_float obj.z, 5653.873
  end

  describe ".[] mixed" do
    it "(int, vec2)" do
      obj = Moon::Vector3[1000, Moon::Vector2.new(14, 15)]
      assert_kind_of Moon::Vector3, obj
      assert_float obj.x, 1000.0
      assert_float obj.y, 14.0
      assert_float obj.z, 15.0
    end

    it "(float, vec2)" do
      obj = Moon::Vector3[7634.8367, Moon::Vector2.new(14, 15)]
      assert_kind_of Moon::Vector3, obj
      assert_float obj.x, 7634.8367
      assert_float obj.y, 14.0
      assert_float obj.z, 15.0
    end

    it "(vec2, int)" do
      obj = Moon::Vector3[Moon::Vector2.new(14, 15), 1000]
      assert_kind_of Moon::Vector3, obj
      assert_float obj.x, 14.0
      assert_float obj.y, 15.0
      assert_float obj.z, 1000.0
    end

    it "(vec2, float)" do
      obj = Moon::Vector3[Moon::Vector2.new(14, 15), 7684.76834]
      assert_kind_of Moon::Vector3, obj
      assert_float obj.x, 14.0
      assert_float obj.y, 15.0
      assert_float obj.z, 7684.76834
    end
  end
end
