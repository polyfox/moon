assert 'Moon::Vector1#initialize' do
  v1 = Moon::Vector1.new
end

assert 'Moon::Vector#x' do
  v1 = Moon::Vector1.new(2)
  assert_equal 2, v1.x
  v1.x = 19
  assert_equal 19, v1.x
end

assert 'Moon::Vector1#-@' do
  v1 = Moon::Vector1.new(1)
  v2 = -v1
  assert_equal(-1, v2.x)
end

assert 'Moon::Vector1#to_i' do
  v1 = Moon::Vector1.new(1.3)
  assert_equal 1, v1.to_i
end

assert 'Moon::Vector1#to_f' do
  v1 = Moon::Vector1.new(1.4)
  assert_float 1.4, v1.to_f.round(1)
end

assert 'Moon::Vector1#to_a' do
  v1 = Moon::Vector1.new(1)
  assert_equal [1], v1.to_a
end
