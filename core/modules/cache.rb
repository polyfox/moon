#
# moon/core/modules/cache.rb
#
module Cache

  @cache = {}
  @constructor = {}

  extend self

  def branch(branch_name)
    define_method(branch_name) do |name, *args|
      key = [name, *args]
      (@cache[branch_name] ||= {})[key] ||= @constructor[branch_name][name].(*args)
    end
    hsh = @constructor[branch_name] ||= {}
    yield hsh
  end

  class << self
    private :branch
  end

  branch :spritesheet do |hsh|
    [[128, 128], [128, 16], [16, 128], [16, 16], [16, 32],
     [256, 256], [32, 16], [32, 32], [8, 8], [96, 96]].each do |ary|
      pair_name = ary.join("x")
      cache_name = "media_buttons_#{pair_name}"
      filename = "resources/" + cache_name + ".png"
      cw, ch = *ary
      hsh[cache_name] = -> do
        Spritesheet.new(filename, cw, ch)
      end
    end
  end

  branch :font do |hsh|
    hsh["ipaexg"] = ->(size) do
      Font.new("resources/fonts/ipaexg/ipaexg.ttf", size)
    end
    hsh["vera"] = ->(size) do
      Font.new("resources/fonts/vera/Vera.ttf", size)
    end
    hsh["vera_mono"] = ->(size) do
      Font.new("resources/fonts/vera/VeraMono.ttf", size)
    end
    hsh["vera_mono_bold_italic"] = ->(size) do
      Font.new("resources/fonts/vera/VeraMoBI.ttf", size)
    end
    hsh["vera_mono_bold"] = ->(size) do
      Font.new("resources/fonts/vera/VeraMoBd.ttf", size)
    end
    hsh["vera_mono_italic"] = ->(size) do
      Font.new("resources/fonts/vera/VeraMoIt.ttf", size)
    end
  end

end
