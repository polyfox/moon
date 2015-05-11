# mruby-moon load
engine = Moon::Engine.new do |e, delta|
  step e, delta
end

begin
  engine.setup

  require 'core/load'
  require 'scripts/load'

  engine.main
ensure
  engine.shutdown
end
