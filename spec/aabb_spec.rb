describe Moon::AABB do
  context ".new" do
    it "should create a new AABB" do
      aabb = Moon::AABB.new(Moon::Vector2.new(3, 3), Moon::Vector2.new(7, 7))
      assert_kind_of(Moon::AABB, aabb)
    end
  end

  context "#intersect?" do
    it "should check if 2 AABBs intersect" do
      aabb1 = Moon::AABB.new(Moon::Vector2.new(3, 3), Moon::Vector2.new(7, 7))
      aabb2 = Moon::AABB.new(Moon::Vector2.new(5, 5), Moon::Vector2.new(7, 7))
      assert_true aabb1.intersect?(aabb2)
    end
  end

  context "#&" do
    it "should generate union AABB from 2 AABBs" do
      aabb1 = Moon::AABB.new(Moon::Vector2.new(3, 3), Moon::Vector2.new(7, 7))
      aabb2 = Moon::AABB.new(Moon::Vector2.new(5, 5), Moon::Vector2.new(7, 7))
      aabb3 = aabb1 & aabb2
      assert_kind_of(Moon::AABB, aabb3)
    end
  end
end
