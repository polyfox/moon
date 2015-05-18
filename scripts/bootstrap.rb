# mruby-moon load
GLFW.init

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
