class State::CoreTest < State::TestBase

  def prep_test
    @texture_from_file = Moon::Texture.new("resources/media_buttons_96x96.png")
    @texture_from_wh = Moon::Texture.new(24, 32)
  end

  def do_test
    assert("Texture|from_file exists", @texture_from_file)
    assert_equal("Texture|from_file #width is equal to 384.0", @texture_from_file.width, 384.0)
    assert_equal("Texture|from_file #height is equal to 288.0", @texture_from_file.height, 288.0)

    assert("Texture|from_wh exists", @texture_from_wh)
    assert_equal("Texture|from_wh #width is equal to 24.0", @texture_from_wh.width, 24.0)
    assert_equal("Texture|from_wh #height is equal to 32.0", @texture_from_wh.height, 32.0)
  end

end