assert 'Moon::Vector4#initialize' do
  v1 = Moon::Vector4.new
end

assert 'Moon::Vector4#to_a' do
  v1 = Moon::Vector4.new(1, 2, 3, 4)
  assert_equal [1, 2, 3, 4], v1.to_a
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
