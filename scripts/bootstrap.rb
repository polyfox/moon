# mruby-moon load
engine = Moon::Engine.new do |e, delta|
  step e, delta
end

engine.setup

require 'core/load'
require 'scripts/load'

engine.main
