#
# LunarMetal
#
module LunarMetal
  VERSION = "0.0.1"#.freeze # you can't freeze objects in mruby D:
end

require 'scripts/lunar_metal/state'
require 'scripts/lunar_metal/game'
require 'scripts/lunar_metal/hud'
require 'scripts/lunar_metal/iso_tilemap'
require 'scripts/lunar_metal/world'