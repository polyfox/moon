assert 'Moon::Vector4#initialize with no parameters' do
  v = Moon::Vector4.new
  assert_equal [0, 0, 0, 0], v.to_a
end

assert 'Moon::Vector4#initialize with 4 parameters' do
  v = Moon::Vector4.new(1, 2, 3, 4)
  assert_equal [1, 2, 3, 4], v.to_a
end

assert 'Moon::Vector4 attribute aliases' do
  v = Moon::Vector4.new(1, 2, 3, 4)
  assert_equal 1, v.x
  assert_equal 2, v.y
  assert_equal 3, v.z
  assert_equal 4, v.w
  assert_equal v.x, v.r
  assert_equal v.y, v.g
  assert_equal v.z, v.b
  assert_equal v.w, v.a
end

assert 'Moon::Vector4#[]' do
  assert_equal [1, 1, 1, 1], Moon::Vector4[1].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[[1, 2, 3, 4]].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[1, [2, 3], 4].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[[1, 2], 3, 4].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[[1, 2, 3], 4].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[1, [2, 3, 4]].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[1, [2], [3], [4]].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[1, 2, 3, 4].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[Moon::Vector1.new(1), 2, 3, 4].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[1, Moon::Vector2.new(2, 3), 4].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[Moon::Vector1.new(1), Moon::Vector2.new(2, 3), 4].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[Moon::Vector3.new(1, 2, 3), 4].to_a
  assert_equal [1, 2, 3, 4], Moon::Vector4[Moon::Vector4.new(1, 2, 3, 4)].to_a
end
