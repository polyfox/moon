describe Moon::DataModel::Base do
  it "should raise a TypeError when given wrong typed objects" do
    assert_raise(TypeError) { Moon::DataModel::Base.new(id: nil) }
  end

  it "should accept correctly typed objects" do
    Moon::DataModel::Base.new(id: "acb635672cdea")
  end
end
