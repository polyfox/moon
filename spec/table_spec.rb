describe Moon::Table do
  context "#subsample" do
    it "should sample a section of a Table" do
      table = Moon::Table.new(12, 12, default: 10)
      subtable = table.subsample(2, 2, 4, 4)
      assert_kind_of(Moon::Table, subtable)
      assert_equal(4, subtable.xsize)
      assert_equal(4, subtable.ysize)
    end
  end
end
