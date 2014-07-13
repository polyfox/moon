describe Moon::PaletteParser do
  it "should load a hex.RGB palette" do
    PaletteParser.load_palette(
      "format" => "hex.RGB",
      "colors" => {
        "black" => "000",
        "white" => "FFF",
        "blue" => "00F",
        "red" => "F00",
        "green" => "0F0"
      }
    )
  end

  it "should load a hex.RRGGBB palette" do
    PaletteParser.load_palette(
      "format" => "hex.RRGGBB",
      "colors" => {
        "black" => "000000",
        "white" => "FFFFFF",
        "blue" => "0000FF",
        "red" => "FF0000",
        "green" => "00FF00"
      }
    )
  end
end
