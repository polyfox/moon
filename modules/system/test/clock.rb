GLFW.init

assert 'Clock type' do
  assert_kind_of(Class, Moon::Clock)
end

assert 'Clock#initialize' do
  clock = Moon::Clock.new
end

assert 'Clock#restart' do
  clock = Moon::Clock.new
  assert_kind_of(Float, clock.restart)
end

assert 'Clock#update' do
  clock = Moon::Clock.new
  clock.update
  true
end

assert 'Clock#fps' do
  clock = Moon::Clock.new
  assert_kind_of(Integer, clock.fps)
end

assert 'Clock#delta' do
  clock = Moon::Clock.new
  assert_kind_of(Float, clock.delta)
end

GLFW.terminate
