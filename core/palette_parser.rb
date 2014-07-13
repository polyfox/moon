module PaletteParser

  class FormatError < StandardError
  end

  def self.load_palette_hex_rgb(colors)
    colors.each_with_object({}) do |a, hash|
      key, hex = *a
      matchdata = hex.match(/[0-9a-fA-F]{3}/)
      raise FormatError, "invalid hex format (#{hex}) expected (RGB)" unless matchdata
      value = matchdata[0].to_i(16)
      r = (value >> 8) & 15
      g = (value >> 4) & 15
      b = value & 15
      hash[key] = Moon::Vector4.new(r/15.0, g/15.0, b/15.0, 1.0)
    end
  end

  def self.load_palette_hex_rrggbb(colors)
    colors.each_with_object({}) do |a, hash|
      key, hex = *a
      matchdata = hex.match(/[0-9a-fA-F]{6}/)
      raise FormatError, "invalid hex format (#{hex}) expected (RRGGBB)" unless matchdata
      value = matchdata[0].to_i(16)
      r = (value >> 16) & 255
      g = (value >> 8) & 255
      b = value & 255
      hash[key] = Moon::Vector4.new(r/255.0, g/255.0, b/255.0, 1.0)
    end
  end

  def self.load_palette_array_rgb(colors)
    colors.each_with_object({}) do |a, hash|
      key, ary = *a
      r, g, b = *ary
      hash[key] = Moon::Vector4.new(r/255.0, g/255.0, b/255.0, 1.0)
    end
  end

  def self.load_palette_array_rgba(colors)
    colors.each_with_object({}) do |a, hash|
      key, ary = *a
      r, g, b, a = *ary
      hash[key] = Moon::Vector4.new(r/255.0,g/255.0,b/255.0,a/255.0)
    end
  end

  def self.load_palette_float_array_rgb(colors)
    colors.each_with_object({}) do |a, hash|
      key, ary = *a
      r, g, b = *ary
      hash[key] = Moon::Vector4.new(r,g,b,1.0)
    end
  end

  def self.load_palette_float_array_rgba(colors)
    colors.each_with_object({}) do |a, hash|
      key, ary = *a
      r, g, b, a = *ary
      hash[key] = Moon::Vector4.new(r,g,b,a)
    end
  end

  def self.load_palette(data)
    colors = data.fetch("colors")
    format = data.fetch("format")
    case format
    when "hex.RGB"
      load_palette_hex_rgb colors
    when "hex.RRGGBB"
      load_palette_hex_rrggbb colors
    when "[rgb]"
      load_palette_array_rgb colors
    when "[rgba]"
      load_palette_array_rgba colors
    when "float[rgb]"
      load_palette_float_array_rgb colors
    when "float[rgba]"
      load_palette_float_array_rgba colors
    else
      raise FormatError, "unsupported palette format #{format}"
    end
  end

end
