class State::CoreTest < State::TestBase

  def run_tests
    test("Texture|from_file") do
      texture_from_file = Moon::Texture.new("resources/media_buttons_96x96.png")

      assert("it exists", texture_from_file)
      assert_equal("it's #width is equal to 384.0", texture_from_file.width, 384.0)
      assert_equal("it's #height is equal to 288.0", texture_from_file.height, 288.0)
    end
    test("Texture|from_wh") do
      texture_from_wh = Moon::Texture.new(24, 32)

      assert("it exists", texture_from_wh)
      assert_equal("it's #width is equal to 24.0", texture_from_wh.width, 24.0)
      assert_equal("it's #height is equal to 32.0", texture_from_wh.height, 32.0)
    end
  end

end