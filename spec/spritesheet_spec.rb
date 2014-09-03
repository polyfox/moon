describe Moon::Spritesheet do
  context "#initialize" do
    it "should initialize from filename" do
      Moon::Spritesheet.new("resources/block_16x16_005.png", 16, 16)
    end

    it "should initialize from Texture" do
      texture = Moon::Texture.new("resources/block_16x16_005.png")
      Moon::Spritesheet.new(texture, 16, 16)
    end
  end
end
