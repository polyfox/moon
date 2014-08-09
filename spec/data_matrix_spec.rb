describe Moon::DataMatrix do
  context "#subsample" do
    it "should sample a section of a DataMatrix" do
      datamatrix = Moon::DataMatrix.new(12, 12, 4, default: 10)
      subdatamatrix = datamatrix.subsample(2, 2, 1, 4, 4, 2)
      assert_kind_of(Moon::DataMatrix, subdatamatrix)
      assert_equal(4, subdatamatrix.xsize)
      assert_equal(4, subdatamatrix.ysize)
      assert_equal(2, subdatamatrix.zsize)
    end
  end
end
