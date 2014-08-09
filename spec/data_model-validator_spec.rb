describe Moon::DataModel::SoftValidator do
  context ".check_type" do
    it "should validate a simple type" do
      Moon::DataModel::SoftValidator.check_type(String, :test, "Hello World")
    end

    it "should validate a composite Array type" do
      Moon::DataModel::SoftValidator.check_type([String], :test, ["Hello World", "My String"])
    end

    it "should validate a composite Hash type" do
      Moon::DataModel::SoftValidator.check_type({ String => Integer }, :test, { "Hello World" => 12, "My String" => 24 })
    end

    it "should error on wrong type" do
      assert_raise(TypeError) do
        Moon::DataModel::SoftValidator.check_type(Integer, :test, "Oh noes")
      end
    end
  end
end

describe Moon::DataModel::VerboseValidator do
  context ".check_type" do
    it "should validate a simple type" do
      Moon::DataModel::VerboseValidator.check_type(String, :test, "Hello World")
    end

    it "should validate a composite array type" do
      Moon::DataModel::VerboseValidator.check_type([String], :test, ["Hello World", "My String"])
    end

    it "should validate a deep nested composite array type" do
      Moon::DataModel::VerboseValidator.check_type([[String]], :test, [["Hello World", "My String"], ["a", "b"]])
    end

    it "should validate a composite hash type" do
      Moon::DataModel::VerboseValidator.check_type({ String => Integer }, :test, { "Hello World" => 12, "My String" => 24 })
    end

    it "should validate a deep nested composite hash type" do
      Moon::DataModel::VerboseValidator.check_type({ String => [Integer] }, :test, { "Hello World" => [12], "My String" => [24] })
    end

    it "should error on wrong type" do
      assert_raise(TypeError) do
        Moon::DataModel::VerboseValidator.check_type(Integer, :test, "Oh noes")
      end
    end

    it "should error on wrong composite Array type" do
      assert_raise(TypeError) do
        Moon::DataModel::VerboseValidator.check_type([Integer], :test, [12, "Oh noes"])
      end
    end

    it "should error on wrong composite Hash type" do
      assert_raise(TypeError) do
        Moon::DataModel::VerboseValidator.check_type({ String => Integer }, :test, { "This" => 12, "Hello" => "World" })
      end
    end
  end
end
