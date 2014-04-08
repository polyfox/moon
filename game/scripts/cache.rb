module Cache

  branch :spritesheet do

    hsh = {}
    [[128, 128], [128, 16], [16, 128], [16, 16], [16, 32],
     [256, 256], [32, 16], [32, 32], [8, 8], [96, 96]].each do |ary|

      pair_name = ary.join("x")
      cache_name = "media_buttons_#{pair_name}"
      filename = "resources/" + cache_name + ".png"
      cw, ch = *ary

      hsh[cache_name] = -> do
        Moon::Spritesheet.new(filename, cw, ch)
      end
    end

    hsh
  end

  branch :font do

    hsh = {}

    hsh["ipaexg"] = ->(size) do
      Moon::Font.new("resources/fonts/ipaexg/ipaexg.ttf", size)
    end

    hsh["vera"] = ->(size) do
      Moon::Font.new("resources/fonts/vera/Vera.ttf", size)
    end

    hsh["vera_mono"] = ->(size) do
      Moon::Font.new("resources/fonts/vera/VeraMono.ttf", size)
    end

    hsh["vera_mono_bold_italic"] = ->(size) do
      Moon::Font.new("resources/fonts/vera/VeraMoBI.ttf", size)
    end

    hsh["vera_mono_bold"] = ->(size) do
      Moon::Font.new("resources/fonts/vera/VeraMoBd.ttf", size)
    end

    hsh["vera_mono_italic"] = ->(size) do
      Moon::Font.new("resources/fonts/vera/VeraMoIt.ttf", size)
    end

    hsh
  end

end