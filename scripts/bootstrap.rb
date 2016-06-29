# mruby-moon load
GLFW.init

engine = Moon::Engine.new do |e, delta|
  step e, delta
end

begin
  engine.setup

  begin
    GC.disable
    require 'core/load' if Dir.exists? 'core'
    require 'scripts/load'
  ensure
    GC.enable
  end

  engine.main
ensure
  engine.shutdown
end
