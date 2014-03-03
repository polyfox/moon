class State::CoreTest < State::TestBase

  def run_tests
    test("Texture|load|from_file") do
      texture_from_file = Moon::Texture.new("resources/media_buttons_96x96.png")

      assert("it exists", texture_from_file)
      assert_equal("it's #width is equal to 384.0", texture_from_file.width, 384.0)
      assert_equal("it's #height is equal to 288.0", texture_from_file.height, 288.0)
    end

    test("Texture|create|from_wh") do
      texture_from_wh = Moon::Texture.new(24, 32)

      assert("it exists", texture_from_wh)
      assert_equal("it's #width is equal to 24.0", texture_from_wh.width, 24.0)
      assert_equal("it's #height is equal to 32.0", texture_from_wh.height, 32.0)
    end

    test("Sprite") do
      sprite = Moon::Sprite.new
      assert("it exists", sprite)
    end

    test("Sprite|change_texture") do
      sprite = Moon::Sprite.new
      texture1 = Moon::Texture.new("resources/media_buttons_96x96.png")
      texture2 = Moon::Texture.new("resources/media_buttons_32x32.png")
      assert_equal("it has a sane default", sprite.texture, nil)
      sprite.texture = texture1
      assert_equal("it changes its texture when assigned", sprite.texture, texture1)
      sprite.texture = texture2
      assert_equal("it changes its texture when assigned again", sprite.texture, texture2)
    end
  end

end