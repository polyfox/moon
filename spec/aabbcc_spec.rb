describe Moon::AABBCC do
  context ".new" do
    it "should create a new AABBCC" do
      aabbcc = Moon::AABBCC.new(Moon::Vector3.new(3, 3, 3), Moon::Vector3.new(7, 7, 7))
      assert_kind_of(Moon::AABBCC, aabbcc)
    end
  end

  context "#intersect?" do
    it "should check if 2 AABBCCs intersect" do
      aabbcc1 = Moon::AABBCC.new(Moon::Vector3.new(3, 3, 3), Moon::Vector3.new(7, 7, 7))
      aabbcc2 = Moon::AABBCC.new(Moon::Vector3.new(5, 5, 5), Moon::Vector3.new(7, 7, 7))
      assert_true aabbcc1.intersect?(aabbcc2)
    end
  end

  context "#&" do
    it "should generate union AABBCC from 2 AABBCCs" do
      aabbcc1 = Moon::AABBCC.new(Moon::Vector3.new(3, 3, 3), Moon::Vector3.new(7, 7, 7))
      aabbcc2 = Moon::AABBCC.new(Moon::Vector3.new(5, 5, 5), Moon::Vector3.new(7, 7, 7))
      aabbcc3 = aabbcc1 & aabbcc2
      assert_kind_of(Moon::AABBCC, aabbcc3)
    end
  end
end
