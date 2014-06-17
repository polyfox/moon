describe Moon::Vector4 do
  it "should create a Vector4 from an Integer" do
    obj = Moon::Vector4[24]
    assert_kind_of Moon::Vector4, obj
    assert_float obj.x, 24.0
    assert_float obj.y, 24.0
    assert_float obj.z, 24.0
    assert_float obj.w, 24.0
  end

  it "should create a Vector4 from a Float" do
    obj = Moon::Vector4[124.8974]
    assert_kind_of Moon::Vector4, obj
    assert_float obj.x, 124.8974
    assert_float obj.y, 124.8974
    assert_float obj.z, 124.8974
    assert_float obj.w, 124.8974
  end

  it "should create a Vector4 from 3 Integer" do
    obj = Moon::Vector4[67, 45, 82, 112]
    assert_kind_of Moon::Vector4, obj
    assert_float obj.x, 67.0
    assert_float obj.y, 45.0
    assert_float obj.z, 82.0
    assert_float obj.w, 112.0
  end

  it "should create a Vector4 from 3 Floats" do
    obj = Moon::Vector4[7686.37649, 1656.3763, 8767.8679, 5463.2768]
    assert_kind_of Moon::Vector4, obj
    assert_float obj.x, 7686.37649
    assert_float obj.y, 1656.3763
    assert_float obj.z, 8767.8679
    assert_float obj.w, 5463.2768
  end

  it "should create a Vector4 from another Vector4" do
    vec2 = Moon::Vector4.new(5623.0, 7887.0, 5653.873, 6874.874)
    obj = Moon::Vector4[vec2]
    assert_kind_of Moon::Vector4, obj
    assert_float obj.x, 5623.0
    assert_float obj.y, 7887.0
    assert_float obj.z, 5653.873
    assert_float obj.w, 6874.874
  end

  describe ".[] mixed" do
    it "(int, vec3)" do
      obj = Moon::Vector4[1000, Moon::Vector3.new(14, 15, 45)]
      assert_kind_of Moon::Vector4, obj
      assert_float obj.x, 1000.0
      assert_float obj.y, 14.0
      assert_float obj.z, 15.0
      assert_float obj.w, 45.0
    end

    it "(float, vec3)" do
      obj = Moon::Vector4[7634.8367, Moon::Vector3.new(14, 15, 32)]
      assert_kind_of Moon::Vector4, obj
      assert_float obj.x, 7634.8367
      assert_float obj.y, 14.0
      assert_float obj.z, 15.0
      assert_float obj.w, 32.0
    end

    it "(int, int, vec2)" do
      obj = Moon::Vector4[56, 74, Moon::Vector2.new(14, 15)]
      assert_kind_of Moon::Vector4, obj
      assert_float obj.x, 56.0
      assert_float obj.y, 74.0
      assert_float obj.z, 14.0
      assert_float obj.w, 15.0
    end

    it "(float, float, vec2)" do
      obj = Moon::Vector4[32.963, 74.3657, Moon::Vector2.new(14, 15)]
      assert_kind_of Moon::Vector4, obj
      assert_float obj.x, 32.963
      assert_float obj.y, 74.3657
      assert_float obj.z, 14.0
      assert_float obj.w, 15.0
    end

    it "(int, vec2, int)" do
      obj = Moon::Vector4[56, Moon::Vector2.new(14, 15), 74]
      assert_kind_of Moon::Vector4, obj
      assert_float obj.x, 56.0
      assert_float obj.y, 14.0
      assert_float obj.z, 15.0
      assert_float obj.w, 74.0
    end

    it "(float, vec2, float)" do
      obj = Moon::Vector4[32.963, Moon::Vector2.new(14, 15), 74.3657]
      assert_kind_of Moon::Vector4, obj
      assert_float obj.x, 32.963
      assert_float obj.y, 14.0
      assert_float obj.z, 15.0
      assert_float obj.w, 74.3657
    end

    it "(vec2, vec2)" do
      obj = Moon::Vector4[Moon::Vector2.new(32, 24), Moon::Vector2.new(14, 15)]
      assert_kind_of Moon::Vector4, obj
      assert_float obj.x, 32.0
      assert_float obj.y, 24.0
      assert_float obj.z, 14.0
      assert_float obj.w, 15.0
    end

    it "(vec2, int, int)" do
      obj = Moon::Vector4[Moon::Vector2.new(14, 15), 56, 74]
      assert_kind_of Moon::Vector4, obj
      assert_float obj.x, 14.0
      assert_float obj.y, 15.0
      assert_float obj.z, 56.0
      assert_float obj.w, 74.0
    end

    it "(vec2, float, float)" do
      obj = Moon::Vector4[Moon::Vector2.new(14, 15), 32.963, 74.3657]
      assert_kind_of Moon::Vector4, obj
      assert_float obj.x, 14.0
      assert_float obj.y, 15.0
      assert_float obj.z, 32.963
      assert_float obj.w, 74.3657
    end

    it "(vec3, int)" do
      obj = Moon::Vector4[Moon::Vector3.new(14, 15, 56), 1000]
      assert_kind_of Moon::Vector4, obj
      assert_float obj.x, 14.0
      assert_float obj.y, 15.0
      assert_float obj.z, 56.0
      assert_float obj.w, 1000.0
    end

    it "(vec3, float)" do
      obj = Moon::Vector4[Moon::Vector3.new(14.7684, 15.3768, 12), 7684.76834]
      assert_kind_of Moon::Vector4, obj
      assert_float obj.x, 14.7684
      assert_float obj.y, 15.3768
      assert_float obj.z, 12.0
      assert_float obj.w, 7684.76834
    end
  end
end
