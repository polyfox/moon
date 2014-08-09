describe Moon::DataModel::Field do
  context ".new" do
    it "should create a new Field" do
      Moon::DataModel::Field.new(type: String, default: "")
    end
  end

  context "#make_default" do
    it "should create a default object" do
      assert_kind_of(Array, Moon::DataModel::Field.new(type: Array, default: proc{|t|t.new}).make_default)
    end
  end

  context "#check_type" do
    it "should validate its type" do
      Moon::DataModel::Field.new(type: String, default: "").check_type(:key, "My String")
    end
  end
end
