class State::CoreTest < State::TestBase

  def run_tests
    test("Texture|load|from_file") do
      texture_from_file = Moon::Texture.new("resources/media_buttons_96x96.png")

      assert("it exists", texture_from_file)
      assert_equal("it's #width is equal to 384.0", texture_from_file.width, 384.0)
      assert_equal("it's #height is equal to 288.0", texture_from_file.height, 288.0)
    end

    #test("Texture|create|from_wh") do
    #  texture_from_wh = Moon::Texture.new(24, 32)
    #  assert("it exists", texture_from_wh)
    #  assert_equal("it's #width is equal to 24.0", texture_from_wh.width, 24.0)
    #  assert_equal("it's #height is equal to 32.0", texture_from_wh.height, 32.0)
    #end

    #test("Sprite") do
    #  sprite = Moon::Sprite.new
    #  assert("it exists", sprite)
    #end

    test("Sprite|texture") do
      sprite = Moon::Sprite.new("resources/media_buttons_16x16.png")
      texture1 = Moon::Texture.new("resources/media_buttons_96x96.png")
      texture2 = Moon::Texture.new("resources/media_buttons_32x32.png")
      assert_class_is_kind_of("it has a sane default", sprite.texture, Moon::Texture)
      sprite.texture = texture1
      assert_equal("it changes its texture when assigned", sprite.texture, texture1)
      sprite.texture = texture2
      assert_equal("it changes its texture when assigned again", sprite.texture, texture2)
      assert_exception("it should error when assigning non-Texture object", TypeError) do
        sprite.texture = "Hello World"
      end
    end

    test("Sprite|clip_rect") do
      sprite = Moon::Sprite.new("resources/media_buttons_16x16.png")
      rect1 = Moon::Rect.new(0, 0, 16, 16)
      rect2 = Moon::Rect.new(16, 0, 16, 16)
      assert_equal("it has a sane default", sprite.clip_rect, nil)
      sprite.clip_rect = rect1
      assert_equal("it changed the clip_rect when assigned", sprite.clip_rect, rect1)
      sprite.clip_rect = rect2
      assert_equal("it changed the clip_rect when assigned again", sprite.clip_rect, rect2)
      assert_exception("it should error when assigning non-Rect object", TypeError) do
        sprite.clip_rect = "I am the walrus"
      end
    end

  end

end